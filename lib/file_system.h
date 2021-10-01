#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include "dir.h"
#include "inode.h"

typedef struct fs {
    dir_item_t path[1024];
    int path_size;

    int inode_index;
    inode_t inode;
} file_system_t;

void fs_init(file_system_t *fs);
void fs_mount(file_system_t *fs);
void fs_curr_path(file_system_t *fs, char str[]);


#endif