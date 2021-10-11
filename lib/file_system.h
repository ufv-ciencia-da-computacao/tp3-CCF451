#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <stdio.h>
#include "disk.h"
#include "bitmap.h"

#define IF_DIR          1
#define IF_FILE         2
#define MAX_FILE_SIZE   65536

typedef struct {
    disk_t *disk;
    int blocks_bitmap_inode;
    int blocks_bitmap_block;
    int blocks_inode;
    int blocks_data;
    bitmap_t b_inode;
    bitmap_t b_block;
} file_system_t;

void fs_init(file_system_t *fs, disk_t *disk);
void fs_flush(file_system_t *fs);
void fs_destroy(file_system_t *fs);
void fs_format(file_system_t *fs);
int  fs_create(file_system_t *fs, int file_type);
int  fs_read(file_system_t *fs, int inode_index, uint8_t *data);
void fs_write(file_system_t *fs, int inode_index, uint8_t *data, int size);
void fs_delete(file_system_t *fs, int inode_index);
int  fs_type(file_system_t *fs, int inode_index);

#endif