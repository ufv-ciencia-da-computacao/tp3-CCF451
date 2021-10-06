#include <string.h>
#include <stdio.h>
#include "../lib/file_system.h"
#include "../lib/inode.h"
#include "../lib/disk.h"
#include "../lib/bitmap.h"
#include <math.h>
#include <assert.h>

#define SUPER_BLOCK_INDEX   1
#define NUMBER_OF_INODES    256

static void read_inode(file_system_t *fs, int inode_index, inode_t *inode) {
    int disk_block = fs->blocks_bitmap_inode + fs->blocks_bitmap_data + 2;          // +1 disk | +1 super_block
    uint8_t *data = (uint8_t *) malloc(fs->blocks_inode * fs->disk->block_size);
    for(int i=0; i<fs->blocks_inode; ++i) {
        disk_read(fs->disk, data + i*fs->disk->block_size, disk_block+i);
    }
    memcpy(inode, data + inode_index * sizeof(inode_t), sizeof(inode_t));
    free(data);
}

static void write_inode(file_system_t *fs, int inode_index, inode_t *inode) {
    int disk_block = fs->blocks_bitmap_inode + fs->blocks_bitmap_data + 2;          // +1 disk | +1 super_block
    uint8_t *data = (uint8_t *) malloc(fs->blocks_inode * fs->disk->block_size);
    for(int i=0; i<fs->blocks_inode; ++i) {
        disk_read(fs->disk, data + i*fs->disk->block_size, disk_block+i);
    }
    memcpy(data + inode_index * sizeof(inode_t), inode, sizeof(inode_t));
    for(int i=0; i<fs->blocks_inode; ++i) {
        disk_write(fs->disk, data + i*fs->disk->block_size, disk_block+i);
    }
    free(data);
}

static void read_bitmap_inode(file_system_t *fs, bitmap_t *b_inode) {
    int disk_block = 2;     // +1 disk | +1 
    uint8_t *data = (uint8_t *) malloc(fs->blocks_bitmap_inode * fs->disk->block_size);
    for(int i=0; i<fs->blocks_bitmap_inode; ++i) {
        disk_read(fs->disk, data + i*fs->disk->block_size, disk_block+i);
    }
    init_bitmap_array(b_inode, data, NUMBER_OF_INODES);
    free(data);
}

static void write_bitmap_inode(file_system_t *fs, bitmap_t *b_inode) {
    int disk_block = 2;     // +1 disk | +1 
    uint8_t *data = (uint8_t *) malloc(fs->blocks_bitmap_inode * fs->disk->block_size);
    for(int i=0; i<fs->blocks_bitmap_inode; ++i) {
        disk_read(fs->disk, data + i*fs->disk->block_size, disk_block+i);
    }
    // copy bitmap to array 
    for(int i=0; i<fs->blocks_bitmap_inode; ++i) {
        disk_write(fs->disk, data + i*fs->disk->block_size, disk_block+i);
    }
    free(data);
}

static void read_bitmap_data(file_system_t *fs, bitmap_t *b_data) {
    int disk_block = fs->blocks_bitmap_inode + 2;     // +1 disk | +1 
    uint8_t *data = (uint8_t *) malloc(fs->blocks_bitmap_data * fs->disk->block_size);
    for(int i=0; i<fs->blocks_bitmap_data; ++i) {
        disk_read(fs->disk, data + i*fs->disk->block_size, disk_block+i);
    }
    init_bitmap_array(b_data, data, fs->disk->nblocks);
    free(data);
}

static void write_bitmap_data(file_system_t *fs, bitmap_t *b_data) {
    int disk_block = fs->blocks_bitmap_inode + 2;     // +1 disk | +1 
    uint8_t *data = (uint8_t *) malloc(fs->blocks_bitmap_data * fs->disk->block_size);
    for(int i=0; i<fs->blocks_bitmap_data; ++i) {
        disk_read(fs->disk, data + i*fs->disk->block_size, disk_block+i);
    }
    // copy bitmap to array 
    for(int i=0; i<fs->blocks_bitmap_data; ++i) {
        disk_write(fs->disk, data + i*fs->disk->block_size, disk_block+i);
    }
    free(data);
}

static void read_super_block(file_system_t *fs) {
    uint8_t *buffer = (uint8_t *) malloc(fs->disk->block_size);
    disk_read(fs->disk, buffer, SUPER_BLOCK_INDEX);
    memcpy(&fs->blocks_bitmap_inode, buffer, sizeof(int));
    memcpy(&fs->blocks_bitmap_data, buffer + sizeof(int), sizeof(int));
    memcpy(&fs->blocks_inode, buffer + 2*sizeof(int), sizeof(int));
    memcpy(&fs->blocks_data, buffer + 3*sizeof(int), sizeof(int));
    free(buffer);
}

static void write_super_block(file_system_t *fs) {
    uint8_t *buffer = (uint8_t *) malloc(fs->disk->block_size);
    memcpy(buffer, &fs->blocks_bitmap_inode, sizeof(int));
    memcpy(buffer + sizeof(int), &fs->blocks_bitmap_data, sizeof(int));
    memcpy(buffer + 2*sizeof(int), &fs->blocks_inode, sizeof(int));
    memcpy(buffer + 3*sizeof(int), &fs->blocks_data, sizeof(int));
    disk_write(fs->disk, buffer, SUPER_BLOCK_INDEX);
    free(buffer);
}

void fs_init(file_system_t *fs, disk_t *disk) {
    fs->disk = disk;
    read_super_block(fs);
    printf("| 1 | 1 | %d | %d | %d | %d |\n", fs->blocks_bitmap_inode, fs->blocks_bitmap_data, fs->blocks_inode, fs->blocks_data);
}

void fs_format(file_system_t *fs) {
    fs->blocks_bitmap_inode = ((NUMBER_OF_INODES+7)/8 + fs->disk->block_size-1) / fs->disk->block_size;
    fs->blocks_bitmap_data = ((fs->disk->nblocks+7)/8 + fs->disk->block_size-1) / fs->disk->block_size;
    fs->blocks_inode = (NUMBER_OF_INODES * sizeof(inode_t) + fs->disk->block_size-1) / fs->disk->block_size;
    fs->blocks_data = fs->disk->nblocks - fs->blocks_inode - fs->blocks_bitmap_inode - fs->blocks_bitmap_data - 2;
    write_super_block(fs);

    int used_blocks = fs->blocks_inode + fs->blocks_bitmap_inode + fs->blocks_bitmap_data + 2;

    bitmap_t b_block;
    init_bitmap(&b_block, fs->disk->nblocks);
    reset_bits(&b_block, used_blocks, fs->disk->nblocks-1);
    set_bits(&b_block, 0, used_blocks-1);
    write_bitmap_data(fs, &b_block);

    bitmap_t b_inode;
    init_bitmap(&b_inode, NUMBER_OF_INODES);
    reset_bits(&b_inode, 0, NUMBER_OF_INODES-1);
    write_bitmap_inode(fs, &b_inode);

    printf("| 1 | 1 | %d | %d | %d | %d |\n", fs->blocks_bitmap_inode, fs->blocks_bitmap_data, fs->blocks_inode, fs->blocks_data);
}

int  fs_create(file_system_t *fs, int file_type) {
    bitmap_t b_inode;
    read_bitmap_inode(fs, &b_inode);

    bitmap_t b_data;
    read_bitmap_data(fs, &b_data);

    int new_inode = next_available_block(&b_inode);
    reset_bits(&b_inode, new_inode, new_inode);

    int av = next_available_block(&b_data);
    set_bits(&b_data, av, av);

    inode_t inode;
    inode_init(&inode, file_type, 0, av, 0, 0);
    write_inode(fs, new_inode, &inode);

    write_bitmap_inode(fs, &b_inode);
    write_bitmap_data(fs, &b_data);
    return new_inode;
}

int  fs_read(file_system_t *fs, int inode_index, uint8_t *data) {
    inode_t inode;
    read_inode(fs, inode_index, &inode);
    time(&inode.last_access_date);
    write_inode(fs, inode_index, &inode);
    if(inode.size == 0) return 0;

    uint8_t *pointers = (uint8_t *) malloc(fs->disk->block_size);
    disk_read(fs->disk, pointers, inode.disk_block_ptr);
    
    int block_index = inode.block_index;
    for(int i=0; i < inode.allocated_blocks; ++i) {
        disk_read(fs->disk, data + i*fs->disk->block_size, block_index);
        memcpy(&block_index, pointers + i*sizeof(int), sizeof(int));
    }
    free(pointers);
    return inode.size;
}

void fs_write(file_system_t *fs, int inode_index, uint8_t *data, int size) {
    bitmap_t b_data;
    read_bitmap_data(fs, &b_data);

    inode_t inode;
    read_inode(fs, inode_index, &inode);

    inode.size = size;
    time(&inode.updated_at);

    uint8_t pointers[4096];
    disk_read(fs->disk, pointers, inode.disk_block_ptr);
    
    int needed_blocks = (size + fs->disk->block_size - 1) / fs->disk->block_size;       // teto(size / BLOCK_SIZE)

    for(; inode.allocated_blocks > needed_blocks; --inode.allocated_blocks) {
        int bit;
        if(inode.allocated_blocks-1 > 0) {
            memcpy(&bit, pointers + sizeof(int) * (inode.allocated_blocks - 1), sizeof(int));
        } else {
            bit = inode.block_index;
        }
        reset_bits(&b_data, bit, bit);
    }

    for(; inode.allocated_blocks < needed_blocks; ++inode.allocated_blocks) {
        int bit;
        bit = next_available_block(&b_data);
        set_bits(&b_data, bit, bit);

        if(inode.allocated_blocks > 0) {
            *(pointers + sizeof(int) * (inode.allocated_blocks-1)) = bit;
        } else {
            inode.block_index = bit;
        }
    }

    assert(inode.allocated_blocks == needed_blocks);

    int block_index = inode.block_index;
    uint8_t *data_ptr = data;
    for(int i=0, offset=0; i < inode.allocated_blocks; ++i, offset += sizeof(int), data_ptr += fs->disk->block_size) {
        disk_write(fs->disk, data_ptr, block_index);
        memcpy(&block_index, pointers + offset, sizeof(int));
    }

    disk_write(fs->disk, pointers, inode.disk_block_ptr);
    write_bitmap_data(fs, &b_data);
    write_inode(fs, inode_index, &inode);
}

void fs_delete(file_system_t *fs, int inode_index) {
    
}