#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include "dir.h"
#include "inode.h"
#include <stdio.h>

typedef struct fs {
    dir_item_t path[1024];
    int path_size;

    int inode_index;
    inode_t inode;

    FILE *f_inode;
    FILE *f_block;
} file_system_t;

void fs_init(file_system_t *fs);
void fs_mount(file_system_t *fs);
void fs_curr_path(file_system_t *fs, char str[]);
void fs_create_dir(file_system_t *fs, char path[]);
void fs_list_dir(file_system_t *fs);


#endif