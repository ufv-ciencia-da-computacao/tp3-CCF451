#ifndef __INODE_H__
#define __INODE_H__

#include <time.h>

#define IF_DIR 1
#define IF_FILE 2

typedef struct inode {
    time_t created_at;
    time_t updated_at;
    time_t last_access_date;
    int    type;
    int    size;
    int    disk_block_ptr;
    int    block_index;                 //block_index[8]
    int    allocated_blocks;
                                        // missing pointers' block
} inode_t;

void inode_init(inode_t *inode, int type, int size, int disk_block_ptr, int block_index, int allocated_blocks);
void inode_set_updated_at(inode_t *inode, time_t t);
void inode_set_last_access_date(inode_t *inode, time_t t);
void inode_set_size(inode_t *inode, int size);
void inode_set_disk_block_ptr(inode_t *inode, int disk_block_ptr);
void inode_set_block_index(inode_t *inode, int block_index);
void inode_set_allocated_blocks(inode_t *inode, int allocated_blocks);
time_t inode_get_created_at(inode_t inode);
time_t inode_get_updated_at(inode_t inode);
time_t inode_get_last_access_date(inode_t inode);
int inode_get_type(inode_t inode);
int inode_get_size(inode_t inode);
int inode_get_disk_block_ptr(inode_t inode);
int inode_get_block_index(inode_t inode);
int inode_get_allocated_blocks(inode_t inode);

#endif