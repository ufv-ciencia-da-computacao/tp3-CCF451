#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lib/color.h"
#include "./lib/cmd_if.h"
#include "./lib/file_system.h"
#include "./lib/inode.h"

int main() {

    file_system_t fs;
#ifdef MOUNT
    fs_mount(&fs);
    printf("Booting for the first time....\n");
#else
    fs_init(&fs);
#endif

    char line[201];
    int argc;
    char *argv[200];

    char path[10000];
    strcpy(path, "/");

    while(1) {
        color_green("SO-TP3");
        color_white(":");
        fs_curr_path(&fs, path);
        color_blue(path);                   // print curr path
        color_white("$ ");
        fgets(line, 200, stdin);
        line[strlen(line) - 1] = '\0';      // remove '\n'

        argv[0] = strtok(line, " ");
        argc = 1;

        while(argv[argc-1] != NULL) {
            argv[argc++] = strtok(NULL, " ");
        }
        argc--;

        if(argc == 0) continue;

        cmd_execute(argc, argv);
    }

    return 0;
}