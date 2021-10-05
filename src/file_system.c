#include <string.h>
#include <stdio.h>
#include "../lib/file_system.h"
#include "../lib/inode.h"
#include "../lib/disk.h"
#include <math.h>

static int next_inode_id = 1;
static int next_block_id = 1;

void fs_init(file_system_t *fs, disk_t *disk) {
    fs->disk = disk;
}

void fs_format(file_system_t *fs);

int  fs_create(file_system_t *fs, int file_type);

int  fs_read(file_system_t *fs, int inode_index, uint8_t *data);
void fs_write(file_system_t *fs, int inode_index, uint8_t *data, int size);
void fs_delete(file_system_t *fs, int inode_index);