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
    while((opt = getopt(argc, argv, ":")) != -1) {
        switch(opt) {
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
    while((opt = getopt(argc, argv, ":")) != -1) {
        switch(opt) {
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
    while((opt = getopt(argc, argv, "r")) != -1) {
        switch(opt) {
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
    while((opt = getopt(argc, argv, "rl")) != -1) {
        switch(opt) {
            case 'i':
                print_inode = 1;
                break;
            case 'l':
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
        printf("\t");
        any = 1;
    }
    if(any) printf("\n");

}

void cmd_cd(int argc, char *argv[]) {
    char opt;
    char name[100] = "/";
    while((opt = getopt(argc, argv, ":")) != -1) {
        switch(opt) {
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

    dir_t dir;
    dir_read(cmd_struct.fs, cmd_struct.inode, &dir);

    for(int i=0; i<dir.nitems; ++i) {
        if(strcmp(dir.items[i].name, name) == 0) {
            int new_inode = dir.items[i].inode;
            cmd_struct.inode = new_inode;
            if(strcmp(name, "..") == 0) cmd_struct.path_size = max(0, cmd_struct.path_size-1);
            else if(strcmp(name, ".")) strcpy(cmd_struct.path[cmd_struct.path_size++], name);
            break;
        }
    }

}

void cmd_touch(int argc, char *argv[]){

  char *name=argv[1];
// open
//   file_create(cmd_struct.fs, cmd_struct.inode, name);

    file_create(cmd_struct.fs, cmd_struct.inode, name);
}

void cmd_mv(int argc, char *argv[]){
  char *name=argv[1];
  char *path=argv[2];

  file_move(cmd_struct.fs, cmd_struct.inode, name, path);
}

void cmd_cat(int argc, char *argv[]){
  char *name=argv[1];

  file_t file;
  file_read(cmd_struct.fs, cmd_struct.inode, name, &file);

  for(int i=0; i<file.size; ++i) printf("%c", file.data[i]);
  printf("\n");
}

void cmd_stat(int argc, char *argv[]){
    char *name=argv[0];
    char *file=argv[1];

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

    file_rename(cmd_struct.fs, cmd_struct.inode, name, new_name);
}

int get_cmd_value(char cmd[]) {
    const char *commands[] = {"exit", "mkdir", "rm", "ls", "clear", "cd","touch","mv","cat","stat","mkfs","copyin","rename"};
    const int cntCommands = 13;

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

void cmd_execute(int argc, char *argv[]) {
    optind = 0;
    switch(get_cmd_value(argv[0])) {
        case 0:
            exit(0);
            break;
        case 1: {
            cmd_mkdir(argc, argv);
            break;
        }
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

        default:
            printf("%s: comando nao encontrado\n", argv[0]);
    }

    fs_flush(cmd_struct.fs);
}
