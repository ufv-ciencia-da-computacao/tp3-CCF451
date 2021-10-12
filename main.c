#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/file_system.h"
#include "lib/cmd_if.h"
#include "lib/color.h"

int main(int argc, char *argv[]) {

    disk_t disk;
    disk_open(&disk, "./files/partition");
    disk_mount(&disk);

    file_system_t fs;
    fs_init(&fs, &disk);
    
    // // disk_format(&disk, 1<<9, 1<<12); fs_format(&fs);

    char line[201];
    int cntArgs;
    char *args[200];

    char path[10000];
    strcpy(path, "/");

    cmd_init(&fs);

    while(1) {
        color_green("SO-TP3");
        color_white(":");
        cmd_path(path);
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