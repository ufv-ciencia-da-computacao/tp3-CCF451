#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "file_system.h"

typedef struct cmd_if {
    file_system_t *fs;
    int inode;

    char path[1000][100];
    int path_size;
}cmd_t;

void cmd_path(char *path);
void cmd_init(file_system_t *fs);
void cmd_execute(char *line);

#endif