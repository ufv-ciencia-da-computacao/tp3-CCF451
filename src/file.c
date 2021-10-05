#include "../lib/file.h"

void file_create(file_system_t *fs, int iinode, char *filename);
void file_rename(file_system_t *fs, int iinode, char *filename, char *new_filename);
void file_move(file_system_t *fs, int iinode, char *filename, char *path);
void file_delete(file_system_t *fs, int iinode, char *filename);
void file_read(file_system_t *fs, int iinode, char *filename, file_t *file);
void file_write(file_system_t *fs, int iinode, char *filename, file_t *file);