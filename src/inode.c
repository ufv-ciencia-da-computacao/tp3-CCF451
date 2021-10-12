#include "../lib/inode.h"

void inode_init(inode_t *inode, int type, int size, int disk_block_ptr, int block_index, int allocated_blocks) {
    inode->type = type;
    inode->size = size;
    inode->disk_block_ptr = disk_block_ptr;
    inode->block_index = block_index;
    inode->allocated_blocks = allocated_blocks;

    inode->created_at = time(NULL);
    inode->last_access_date = time(NULL);
    inode->updated_at = time(NULL);
}

void inode_set_updated_at(inode_t *inode, time_t t) {
    inode->updated_at = t;
}

void inode_set_last_access_date(inode_t *inode, time_t t) {
    inode->last_access_date = t;
}

void inode_set_size(inode_t *inode, int size) {
    inode->size = size;
}

void inode_set_disk_block_ptr(inode_t *inode, int disk_block_ptr) {
    inode->disk_block_ptr = disk_block_ptr;
}

void inode_set_block_index(inode_t *inode, int block_index) {
    inode->block_index = block_index;
}

void inode_set_allocated_blocks(inode_t *inode, int allocated_blocks) {
    inode->allocated_blocks = allocated_blocks;
}

time_t inode_get_created_at(inode_t inode) {
    return inode.created_at;
}

time_t inode_get_updated_at(inode_t inode) {
    return inode.updated_at;
}

time_t inode_get_last_access_date(inode_t inode) {
    return inode.last_access_date;
}

int inode_get_type(inode_t inode) {
    return inode.type;
}

int inode_get_size(inode_t inode) {
    return inode.size;
}

int inode_get_disk_block_ptr(inode_t inode) {
    return inode.disk_block_ptr;
}

int inode_get_block_index(inode_t inode) {
    return inode.block_index;
}

int inode_get_allocated_blocks(inode_t inode) {
    return inode.allocated_blocks;
}