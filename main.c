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

    char path[10000];
    strcpy(path, "/");
    char line[201];

    cmd_init(&fs);

    while(1) {
        color_green("SO-TP3");
        color_white(":");
        cmd_path(path);
        color_blue(path);
        color_white("$ ");
        fgets(line, 200, stdin);
        cmd_execute(line);
    }

    return 0;
}