#include "../lib/dir.h"
#include <string.h>

void dir_create(file_system_t *fs, int iinode, char *dirname);
void dir_rename(file_system_t *fs, int iinode, char *dirname, char *new_dirname);
void dir_delete(file_system_t *fs, int iinode, char *dirname);
void dir_read(file_system_t *fs, int iinode, char *dirname, dir_t *dir);
void dir_write(file_system_t *fs, int iinode, char *dirname, dir_t *dir);
int  dir_open(file_system_t *fs, int iinode, char *dirname);