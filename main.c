#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lib/color.h"
#include "./lib/commands.h"

int main() {
    char line[201];
    int argc;
    char *argv[200];

    while(1) {
        color_green("SO-TP3");
        color_white(":");
        color_blue("/");                    // print curr path
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