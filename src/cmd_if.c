#include "../lib/cmd_if.h"
#include "../lib/dir.h"
#include "../lib/file.h"
#include "../lib/file_system.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCKSIZE 1<<9
#define NUM_BLOCKS 1<<12
cmd_t cmd_struct;

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

    if(optind == argc) {
        
        // fs_list_dir(filesystem);

    } else {
        for(; optind < argc; ++optind) {
            printf("list dir %s\n", argv[optind]);
        }
    }

}

void cmd_cd(int argc, char *argv[]) {
    char opt;
    char name[28] = "/";
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

    // fs_open_dir(filesystem, name);
}

void cmd_touch(int argc, char *argv[]){

  char *name=argv[0];
  char *path=argv[1];
// open
//   file_create(cmd_struct.fs, cmd_struct.inode, name);

  printf("name:%s\n",name );
  printf("path:%s\n", path);

}

void cmd_mv(int argc, char *argv[]){
  char *name=argv[0];
  char *file=argv[1];
  char *path=argv[2];

  printf("name:%s file:%s path:%s\n",name,file,path );


}

void cmd_cat(int argc, char *argv[]){
  char *name=argv[0];
  char *file=argv[1];

  printf("name:%s file:%s \n",name,file );
}

void cmd_stat(int argc, char *argv[]){
  char *name=argv[0];
  char *file=argv[1];

  printf("name:%s file:%s \n",name,file );
}

int get_cmd_value(char cmd[]) {
    const char *commands[] = {"exit", "mkdir", "rm", "ls", "clear", "cd","touch","mv","cat","stat"};
    const int cntCommands = 10;

    for(int i=0; i<cntCommands; ++i) {
        if(strcmp(cmd, commands[i]) == 0) return i;
    }
    return -1;
}

void cmd_init(file_system_t *file_not_global) {    
    cmd_struct.fs = file_not_global;
    cmd_struct.inode = 0;
    strcpy(cmd_struct.current_dirname, "/");

    disk_format(cmd_struct.fs->disk, BLOCKSIZE, NUM_BLOCKS);
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

        default:
            printf("%s: comando nao encontrado\n", argv[0]);
    }

    fs_flush(cmd_struct.fs);
}
