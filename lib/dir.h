#ifndef __DIR_H__
#define __DIR_H__

#include "file_system.h"

typedef struct {
    char name[28];
    int inode;
} dir_item_t;

typedef struct {
    dir_item_t items[MAX_FILE_SIZE / 32];
    int nitems;
} dir_t;

dir_item_t dir_item_create(char *dirname, int inode);
void dir_create(file_system_t *fs, int iinode, char *dirname);
void dir_rename(file_system_t *fs, int iinode, char *dirname, char *new_dirname);
void dir_delete(file_system_t *fs, int iinode, char *dirname);
void dir_read(file_system_t *fs, int iinode, char *dirname, dir_t *dir);
void dir_write(file_system_t *fs, int iinode, char *dirname, dir_t *dir);
int  dir_open(file_system_t *fs, int iinode, char *dirname);

#endif