#include "../lib/commands.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        printf("Criar dir %s\n", name);
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
            printf("Remove dir %s\n", name);
        } else {
            printf("Remove file %s\n", name);
        }
    }

}

void cmd_ls(int argc, char *argv[]) {

}

int get_cmd_value(char cmd[]) {
    const char *commands[] = {"exit", "mkdir", "rm", "ls", "clear"};
    const int cntCommands = 5;

    for(int i=0; i<cntCommands; ++i) {
        if(strcmp(cmd, commands[i]) == 0) return i;
    }
    return -1;
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
            break;
        case 4:
            system("clear");
            break;
        default:
            printf("%s: comando nao encontrado\n", argv[0]);
    }
}