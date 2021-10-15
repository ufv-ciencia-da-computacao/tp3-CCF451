#include "../lib/cmd_if.h"
#include "../lib/dir.h"
#include "../lib/file.h"
#include "../lib/file_system.h"
#include "../lib/color.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a, b) (a > b ? a : b)
#define BLOCKSIZE 1<<9
#define NUM_BLOCKS 1<<12
cmd_t cmd_struct;

void cmd_path(char *path) {
    if(cmd_struct.path_size == 0) strcpy(path, "/");
    else {
        strcpy(path, "");
        for(int i=0; i<cmd_struct.path_size; ++i) {
            strcat(path, "/");
            strcat(path, cmd_struct.path[i]);
        }
    }
}

void cmd_mkfs(int argc, char *argv[]) {
    char opt;
    char *name;
    while((opt = getopt(argc, argv, ":v")) != -1) {
        switch(opt) {
            case 'v':
                printf("mkfs\n");
                printf("Aqui o disco é formatado com os valores passados na entrada\n");
                printf("O sistema de arquivos é formatado para usar os novos valores atribuidos ao disco\n");
                printf("O diretorio raiz é criado e escrito no sistema de arquivos\n");
                break;
            default:
                return;
        }
    }

    if(argc - optind < 2) {
        fprintf(stderr, "%s: faltando operando\n", argv[0]);
        return;
    }

    // mkfs blocksize num_blocks
    int blocksize = atoi(argv[optind++]);
    int n_blocks = atoi(argv[optind++]);

    for(; optind < argc; ++optind) {
        
    }

    disk_format(cmd_struct.fs->disk, blocksize, n_blocks);
    fs_format(cmd_struct.fs);

    dir_t dir;
    int inode = fs_create(cmd_struct.fs, IF_DIR);
    dir.items[0] = dir_item_create(".", inode);
    dir.items[1] = dir_item_create("..", inode);
    dir.nitems = 2;

    cmd_struct.inode = inode;

    fs_write(cmd_struct.fs, inode, (uint8_t*)dir.items, dir.nitems*sizeof(dir_item_t));
    fs_flush(cmd_struct.fs);
}

void cmd_mkdir(int argc, char *argv[]) {
    char opt;
    char *name;
    while((opt = getopt(argc, argv, ":v")) != -1) {
        switch(opt) {
            case 'v':
                printf("mkdir\n");
                printf("Usando o inode do diretorio atual, o sistema de arquivos cria um novo arquivo com propriedades de diretorio\n");
                printf("Os diretorios . e .. são adicionados nesse arquivo\n");
                printf("Os blocos usados para o diretorio são marcados no bitmap como blocos indisponíveis\n");
                break;
            default:
                return;
        }
    }

    if(argc - optind == 0) {
        fprintf(stderr, "%s: faltando operando\n", argv[0]);
        return;
    }

    for(; optind < argc; ++optind) {
        name = argv[optind];
        
        dir_create(cmd_struct.fs, cmd_struct.inode, name);
    }

}

void cmd_rm(int argc, char *argv[]) {
    char opt;
    int remove_dir = 0;
    char *name = NULL;
    while((opt = getopt(argc, argv, ":rv")) != -1) {
        switch(opt) {
            case 'v':
                printf("rm\n");
                printf("Esse comando pode remover arquivos ou diretórios\n");
                printf("No modo recursivo ele itera sobre os subdiretórios e chama recursivamente para aqueles que são diretórios\n");
                printf("Os blocos removidos são marcados como disponíveis no bitmap\n");
                break;
            case 'r':
                remove_dir = 1;
                break;
            default:
                return;
        }
    }
    if(argc - optind == 0) {
        fprintf(stderr, "%s: faltando operando\n", argv[0]);
        return;
    }

    for(; optind < argc; ++optind) {
        name = argv[optind];
        if(remove_dir) {
            dir_delete(cmd_struct.fs, cmd_struct.inode, name);
            // printf("Remove dir %s\n", name);
        } else {
            file_delete(cmd_struct.fs, cmd_struct.inode, name);
            // printf("Remove file %s\n", name);
        }
    }

}

void cmd_ls(int argc, char *argv[]) {
    char opt;
    char *name = NULL;
    int print_inode = 0;
    while((opt = getopt(argc, argv, ":iv")) != -1) {
        switch(opt) {
            case 'v':
                printf("ls\n");
                printf("Aqui todo o conteúdo de um diretório é buscado e mostrado na tela\n");
                printf("Para fazer a leitura, acessamos o disco pelos blocos do inode atual\n");
                printf("Buacamos todo os itens de diretório e apresentamos na tela\n");
                printf("E possível ver os inodes de cada arquivo usando -i\n");
                break;
            case 'i':
                print_inode = 1;
                break;
            default:
                return;
        }
    }

    dir_t dir;
    dir_read(cmd_struct.fs, cmd_struct.inode, &dir);
    int any = 0;

    for(int i=2; i<dir.nitems; ++i) {
        if(fs_type(cmd_struct.fs, dir.items[i].inode) == IF_DIR) color_blue(dir.items[i].name);
        else color_green(dir.items[i].name);
        if(print_inode) {
            color_white("");
            printf(" %d", dir.items[i].inode);
        }
        printf("\t\t");
        any = 1;
    }
    if(any) printf("\n");

}

void cmd_cd(int argc, char *argv[]) {
    char name[200] = "/";
    char opt;
    while((opt = getopt(argc, argv, ":v")) != -1) {
        switch(opt) {
            case 'v':
                printf("cd\n");
                printf("Para abrir um diretório, separamos o nome pelos caracteres '/'\n");
                printf("Se o primeiro caractere for '/', então o caminho é absoluto. Senão, e relativo\n");
                printf("Os nomes são lidos um a um e a cada leitura, o sudiretorio e aberto\n");
                printf("Ao final, temos um diretório aberto\n");
                break;
            default:
                printf("%s: argumetos invalidos\n", argv[0]);
                return;
        }
    }

    if(argc - optind >= 2) {
        printf("%s: muitos argumentos\n", argv[0]);
        return;
    }

    if(argc - optind == 1) {
        strcpy(name, argv[optind]);
    }

    char *tok = strtok(argv[1], "/");
    // printf("%ld\n", tok);
    if(tok == NULL) {
        cmd_struct.path_size = 0;
        cmd_struct.inode = 0;    
        return;
    }

    if(strcmp(tok, "") == 0) {
        cmd_struct.path_size = 0;
        cmd_struct.inode = 0;    
        tok = strtok(NULL, "/");
    }

    while(tok != NULL) {
        int new_inode = dir_open(cmd_struct.fs, cmd_struct.inode, tok);
        if(new_inode == -1) break;
        cmd_struct.inode = new_inode;
        if(strcmp(tok, "..") == 0) cmd_struct.path_size = max(0, cmd_struct.path_size-1);
        else if(strcmp(tok, ".")) strcpy(cmd_struct.path[cmd_struct.path_size++], tok);
        tok = strtok(NULL, "/");
    }

}

void cmd_touch(int argc, char *argv[]){

  char *name=argv[1];
  char opt;
    while((opt = getopt(argc, argv, ":v")) != -1) {
        switch(opt) {
            case 'v':
                printf("touch\n");
                printf("Usamos o sistema de arquivos para criar um arquivo, ou seja, usar um inode disponivel para um novo arquivo\n");
                printf("Iniciamente o arquivo e vazio\n");
                break;
            default:
                printf("%s: argumetos invalidos\n", argv[0]);
                return;
        }
    }
// open
//   file_create(cmd_struct.fs, cmd_struct.inode, name);

    file_create(cmd_struct.fs, cmd_struct.inode, name);
}

void cmd_mv(int argc, char *argv[]){
  char *name=argv[1];
  char *path=argv[2];
  char opt;
    while((opt = getopt(argc, argv, ":v")) != -1) {
        switch(opt) {
            case 'v':
                printf("mv\n");
                printf("Move um arquivo do diretorio atual para algum outro diretorio ou arquivo\n");
                printf("Usa o mesmo processo do comando cd para abrir o diretorio destino\n");
                printf("Um novo arquivo e criado\n");
                printf("Em seguida os dados do arquivo atual sao copiados para o novo arquivo criado\n");
                printf("Por fim, os dados do arquivo atual sao apagados\n");
                break;
            default:
                printf("%s: argumetos invalidos\n", argv[0]);
                return;
        }
    }

  file_move(cmd_struct.fs, cmd_struct.inode, name, path);
}

void cmd_cat(int argc, char *argv[]){
  char *name=argv[1];
  char opt;
    while((opt = getopt(argc, argv, ":v")) != -1) {
        switch(opt) {
            case 'v':
                printf("cat\n");
                printf("E feita uma leitura do disco para recuperar o arquivo\n");
                printf("Os dados lidos são mostrados na tela\n");
                break;
            default:
                printf("%s: argumetos invalidos\n", argv[0]);
                return;
        }
    }

  file_t file;
  file_read(cmd_struct.fs, cmd_struct.inode, name, &file);

  for(int i=0; i<file.size; ++i) printf("%c", file.data[i]);
  printf("\n");
}

void cmd_stat(int argc, char *argv[]){
    char *name=argv[0];
    char *file=argv[1];
    char opt;
    while((opt = getopt(argc, argv, ":v")) != -1) {
        switch(opt) {
            case 'v':
                printf("stat\n");
                printf("Esse comando lê os dados do inode do arquivo ou diretorio desejado\n");
                printf("Os dados lidos são apresentados na tela\n");
                break;
            default:
                printf("%s: argumetos invalidos\n", argv[0]);
                return;
        }
    }

    dir_t dir;
    dir_read(cmd_struct.fs, cmd_struct.inode, &dir);
    int inode;

    for(int i=0; i<dir.nitems; i++) {
        if (!strcmp(dir.items[i].name, file)) {
            inode = dir.items[i].inode;
            break;
        }
    }

    time_t last_access = fs_inode_get_last_access_date(cmd_struct.fs, inode);
    time_t created_at = fs_inode_get_created_at(cmd_struct.fs, inode);
    time_t updated_at = fs_inode_get_updated_at(cmd_struct.fs, inode);

    struct tm *t1, *t2, *t3;
    t1 = localtime(&last_access);
    printf("Last Access: %s",asctime(t1));
    t2 = localtime(&created_at);
    printf("Created At: %s", asctime(t2));
    t3 = localtime(&updated_at); 
    printf("Updated At: %s", asctime(t3));
    fflush(stdin);
    printf("Filetype: %s\n", fs_inode_get_type(cmd_struct.fs, inode) == IF_DIR ? "Directory": "File");
    printf("Allocated blocks: %d\n", fs_inode_get_allocated_blocks(cmd_struct.fs, inode));
    printf("Block index: %d\n", fs_inode_get_block_index(cmd_struct.fs, inode));
    printf("Size: %d\n", fs_inode_get_size(cmd_struct.fs, inode));
}

void cmd_copyin(int argc, char *argv[]) {
    char *name = argv[1];
    char *real = argv[2];
    char opt;
    while((opt = getopt(argc, argv, ":v")) != -1) {
        switch(opt) {
            case 'v':
                printf("copyin\n");
                printf("Com esse comando copiamos os dados de um arquivo real na maquina para dentro de uma arquivo simulado\n");
                printf("E feita a leitura do arquivo real com no maximo 65536 bytes\n");
                printf("Os dados são escritos no arquivo de destino, alocando ou apagando blocos quando necesario\n");
                break;
            default:
                printf("%s: argumetos invalidos\n", argv[0]);
                return;
        }
    }

    FILE *f = fopen(real, "r");
    file_t file;
    file.size = 0;

    for(int i=0; i<MAX_FILE_SIZE; ++i) {
        char c;
        fscanf(f, "%c", &c);
        if(feof(f)) break;
        file.data[file.size++] = c;
    }

    fclose(f);

    file_write(cmd_struct.fs, cmd_struct.inode, name, &file);
}

void cmd_rename(int argc, char *argv[]) {
    char *name = argv[1];
    char *new_name = argv[2];
    char opt;
    while((opt = getopt(argc, argv, ":v")) != -1) {
        switch(opt) {
            case 'v':
                printf("rename\n");
                printf("Nesse comando, percorremos o diretorio atual em busca do nome atual\n");
                printf("Quando é encontrado, subtituimos pelo novo nome\n");
                printf("Os nomes de arquivos e diretorios podem ter ate 17 caracteres\n");
                printf("Os nomes nao podem conter espacos\n");
                break;
            default:
                printf("%s: argumetos invalidos\n", argv[0]);
                return;
        }
    }

    file_rename(cmd_struct.fs, cmd_struct.inode, name, new_name);
}

void cmd_run(int argc, char *argv[]) {
    char *name = argv[1];
    char opt;
    while((opt = getopt(argc, argv, ":v")) != -1) {
        switch(opt) {
            case 'v':
                printf("run\n");
                printf("Esse comando le um arquivo real de entrada e redireciona as linhas para dentro do sistema\n");
                printf("Cada linha é entao executada\n");
                break;
            default:
                printf("%s: argumetos invalidos\n", argv[0]);
                return;
        }
    }

    FILE *f = fopen(name, "r");
    char line[200];
    while(fgets(line, 199, f)) {
        cmd_execute(line);
    }

    fclose(f);
}

int get_cmd_value(char cmd[]) {
    const char *commands[] = {"exit", "mkdir", "rm", "ls", "clear", "cd","touch","mv","cat","stat","mkfs","copyin","rename", "run"};
    const int cntCommands = 14;

    for(int i=0; i<cntCommands; ++i) {
        if(strcmp(cmd, commands[i]) == 0) return i;
    }
    return -1;
}

void cmd_init(file_system_t *file_not_global) {    
    cmd_struct.fs = file_not_global;
    cmd_struct.inode = 0;
    cmd_struct.path_size = 0;

    // disk_format(cmd_struct.fs->disk, BLOCKSIZE, NUM_BLOCKS);
    // fs_format(cmd_struct.fs);

    // dir_t dir;
    // int inode = fs_create(cmd_struct.fs, IF_DIR);
    // dir.items[0] = dir_item_create(".", inode);
    // dir.items[1] = dir_item_create("..", inode);
    // dir.nitems = 2;

    // cmd_struct.inode = inode;

    // fs_write(cmd_struct.fs, inode, (uint8_t*)dir.items, dir.nitems*sizeof(dir_item_t));
    // fs_flush(cmd_struct.fs);
}

void cmd_execute(char *line) {

    int len = strlen(line);
    if(len == 0) return;

    int argc;
    char *argv[200];

    while(line[len-1] == '\n' || line[len-1] == '\r') line[--len] = '\0';      // remove '\n'

    fprintf(stderr, "%s.\n", line);

    argv[0] = strtok(line, " ");
    argc = 1;

    while(argv[argc-1] != NULL) {
        argv[argc++] = strtok(NULL, " ");
    }
    argc--;

    if(argc == 0) return;

    optind = 0;
    switch(get_cmd_value(argv[0])) {
        case 0:
            exit(0);
            break;
        case 1:
            cmd_mkdir(argc, argv);
            break;
        case 2:
            cmd_rm(argc, argv);
            break;
        case 3:
            cmd_ls(argc, argv);
            break;
        case 4:
            system("clear");
            break;
        case 5:
            cmd_cd(argc, argv);
            break;
        case 6:
            cmd_touch(argc,argv);
            break;
        case 7:
            cmd_mv(argc,argv);
            break;
        case 8:
            cmd_cat(argc,argv);
            break;
        case 9:
            cmd_stat(argc,argv);
            break;
        case 10:
            cmd_mkfs(argc,argv);
            break;
        case 11:
            cmd_copyin(argc, argv);
            break;
        case 12:
            cmd_rename(argc, argv);
            break;
        case 13:
            cmd_run(argc, argv);
            break;

        default:
            printf("%s: comando nao encontrado\n", argv[0]);
    }

    fs_flush(cmd_struct.fs);
}
