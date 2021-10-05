#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/file_system.h"

int main(int argc, char *argv[]) {

    disk_t disk;
    disk_open(&disk, "files/partition");
    disk_format(&disk, 1024, 65536);
    disk_mount(&disk);

    


    // char line[201];
    // int cntArgs;
    // char *args[200];

    // char path[10000];
    // strcpy(path, "/");

    // cmd_init(&fs);

    // while(1) {
    //     color_green("SO-TP3");
    //     color_white(":");
    //     fs_curr_path(&fs, path);
    //     color_blue(path);                   // print curr path
    //     color_white("$ ");
    //     fgets(line, 200, stdin);
    //     line[strlen(line) - 1] = '\0';      // remove '\n'

    //     args[0] = strtok(line, " ");
    //     cntArgs = 1;

    //     while(args[cntArgs-1] != NULL) {
    //         args[cntArgs++] = strtok(NULL, " ");
    //     }
    //     cntArgs--;

    //     if(cntArgs == 0) continue;

    //     cmd_execute(cntArgs, args);
    // }

    return 0;
}