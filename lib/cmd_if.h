#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "file_system.h"

void cmd_init(file_system_t *fs);
void cmd_execute(int argc, char *argv[]);


#endif