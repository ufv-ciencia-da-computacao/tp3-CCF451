#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lib/color.h"
#include "./lib/cmd_if.h"
#include "./lib/file_system.h"
#include "./lib/inode.h"

int main(int argc, char *argv[]) {

    file_system_t fs;

    if(argc == 2 && strcmp(argv[1], "mount") == 0) {
        fs_mount(&fs);
        printf("Booting for the first time....\n");
    } else {
        fs_init(&fs);
    }

    char line[201];
    int cntArgs;
    char *args[200];

    char path[10000];
    strcpy(path, "/");

    cmd_init(&fs);

    while(1) {
        color_green("SO-TP3");
        color_white(":");
        fs_curr_path(&fs, path);
        color_blue(path);                   // print curr path
        color_white("$ ");
        fgets(line, 200, stdin);
        line[strlen(line) - 1] = '\0';      // remove '\n'

        args[0] = strtok(line, " ");
        cntArgs = 1;

        while(args[cntArgs-1] != NULL) {
            args[cntArgs++] = strtok(NULL, " ");
        }
        cntArgs--;

        if(cntArgs == 0) continue;

        cmd_execute(cntArgs, args);
    }

    return 0;
}