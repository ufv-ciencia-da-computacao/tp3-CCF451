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
    int disk_block = fs->blocks_bitmap_inode + fs->blocks_bitmap_block + 2;          // +1 disk | +1 super_block
    uint8_t *data = (uint8_t *) malloc(fs->blocks_inode * fs->disk->block_size);
    for(int i=0; i<fs->blocks_inode; ++i) {
        disk_read(fs->disk, data + i*fs->disk->block_size, disk_block+i);
    }
    memcpy(inode, data + inode_index * sizeof(inode_t), sizeof(inode_t));
    free(data);
}

static void write_inode(file_system_t *fs, int inode_index, inode_t *inode) {
    int disk_block = fs->blocks_bitmap_inode + fs->blocks_bitmap_block + 2;          // +1 disk | +1 super_block
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

static void read_super_block(file_system_t *fs) {
    uint8_t *buffer = (uint8_t *) malloc(fs->disk->block_size);
    disk_read(fs->disk, buffer, SUPER_BLOCK_INDEX);
    memcpy(&fs->blocks_bitmap_inode, buffer, sizeof(int));
    memcpy(&fs->blocks_bitmap_block, buffer + sizeof(int), sizeof(int));
    memcpy(&fs->blocks_inode, buffer + 2*sizeof(int), sizeof(int));
    memcpy(&fs->blocks_data, buffer + 3*sizeof(int), sizeof(int));
    free(buffer);
}

static void write_super_block(file_system_t *fs) {
    uint8_t *buffer = (uint8_t *) malloc(fs->disk->block_size);
    memcpy(buffer, &fs->blocks_bitmap_inode, sizeof(int));
    memcpy(buffer + sizeof(int), &fs->blocks_bitmap_block, sizeof(int));
    memcpy(buffer + 2*sizeof(int), &fs->blocks_inode, sizeof(int));
    memcpy(buffer + 3*sizeof(int), &fs->blocks_data, sizeof(int));
    disk_write(fs->disk, buffer, SUPER_BLOCK_INDEX);
    free(buffer);
}

void debug_arr(uint8_t *data, int len) {
    for(int i=0; i<len; ++i) {
        fprintf(stderr, "%d.", data[i]);
    }
    fprintf(stderr, "\n");
}

static void read_bitmaps(file_system_t *fs) {
    fprintf(stderr, "read_bitmaps()\n");
    // Read inode bitmap //
    int block_index = 2;
    uint8_t *buffer = (uint8_t *) malloc(fs->blocks_bitmap_inode * fs->disk->block_size);
    for(int i=0; i<fs->blocks_bitmap_inode; ++i) {
        disk_read(fs->disk, buffer + i*fs->disk->block_size, block_index+i);
    }
    debug_arr(buffer, fs->disk->block_size);
    init_bitmap(&fs->b_inode, NUMBER_OF_INODES);
    for(int bit=0, i=0, j=0; bit<NUMBER_OF_INODES; ++bit, j++) {
        if(j == 8) {
            j=0;
            ++i;
        }
        if(buffer[i] & (1 << j))    set_bits(&fs->b_inode, bit, bit);
        else                        reset_bits(&fs->b_inode, bit, bit);
    }
    free(buffer);

    // Read block bitmap //
    block_index = 2 + fs->blocks_bitmap_inode;
    buffer = (uint8_t *) malloc(fs->blocks_bitmap_block * fs->disk->block_size);
    for(int i=0; i<fs->blocks_bitmap_block; ++i) {
        disk_read(fs->disk, buffer + i*fs->disk->block_size, block_index+i);
    }
    debug_arr(buffer, fs->disk->block_size);
    init_bitmap(&fs->b_block, fs->disk->nblocks);
    for(int bit=0, i=0, j=0; bit<NUMBER_OF_INODES; ++bit, ++j) {
        if(j == 8) {
            j=0;
            ++i;
        }
        if(buffer[i] & (1 << j))    set_bits(&fs->b_block, bit, bit);
        else                        reset_bits(&fs->b_block, bit, bit);
    }
    free(buffer);
}

static void write_bitmaps(file_system_t *fs) {
    fprintf(stderr, "write_bitmaps()\n");
    // write inode bitmap //
    int block_index = 2;
    uint8_t *buffer = (uint8_t *) malloc(fs->blocks_bitmap_inode * fs->disk->block_size);
    memset(buffer, 0, fs->blocks_bitmap_inode * fs->disk->block_size);
    for(int bit=0, i=0, j=0; bit<NUMBER_OF_INODES; ++bit, j++) {
        if(j == 8) {
            j=0;
            ++i;
        }
        if(get_bit(&fs->b_inode, bit))  buffer[i] |= (1 << j);
    }
    for(int i=0; i<fs->blocks_bitmap_inode; ++i) {
        disk_write(fs->disk, buffer + i*fs->disk->block_size, block_index+i);
    }
    free(buffer);

    // Write block bitmap //
    block_index = 2 + fs->blocks_bitmap_inode;
    buffer = (uint8_t *) malloc(fs->blocks_bitmap_block * fs->disk->block_size);
    memset(buffer, 0, fs->blocks_bitmap_block * fs->disk->block_size);
    for(int bit=0, i=0, j=0; bit<fs->disk->nblocks; ++bit, j++) {
        if(j == 8) {
            j=0;
            ++i;
        }
        if(get_bit(&fs->b_block, bit))  buffer[i] |= (1 << j);
    }
    for(int i=0; i<fs->blocks_bitmap_block; ++i) {
        disk_write(fs->disk, buffer + i*fs->disk->block_size, block_index+i);
    }
    free(buffer);
}

void fs_init(file_system_t *fs, disk_t *disk) {
    fprintf(stderr, "fs_init()\n");
    fs->disk = disk;
    read_super_block(fs);
    read_bitmaps(fs);
    fprintf(stderr, "| 1 | 1 | %d | %d | %d | %d |\n", fs->blocks_bitmap_inode, fs->blocks_bitmap_block, fs->blocks_inode, fs->blocks_data);
}

void fs_flush(file_system_t *fs) {
    write_bitmaps(fs);
}

void fs_destroy(file_system_t *fs) {
    destroy_bitmap(&fs->b_inode);
    destroy_bitmap(&fs->b_block);
}

void fs_format(file_system_t *fs) {
    fprintf(stderr, "fs_format()\n");
    fs->blocks_bitmap_inode = ((NUMBER_OF_INODES+7)/8 + fs->disk->block_size-1) / fs->disk->block_size;
    fs->blocks_bitmap_block = ((fs->disk->nblocks+7)/8 + fs->disk->block_size-1) / fs->disk->block_size;
    fs->blocks_inode = (NUMBER_OF_INODES * sizeof(inode_t) + fs->disk->block_size-1) / fs->disk->block_size;
    fs->blocks_data = fs->disk->nblocks - fs->blocks_inode - fs->blocks_bitmap_inode - fs->blocks_bitmap_block - 2;
    write_super_block(fs);

    int used_blocks = fs->blocks_inode + fs->blocks_bitmap_inode + fs->blocks_bitmap_block + 2;

    destroy_bitmap(&fs->b_block);
    destroy_bitmap(&fs->b_inode);

    init_bitmap(&fs->b_block, fs->disk->nblocks);
    init_bitmap(&fs->b_inode, NUMBER_OF_INODES);

    fprintf(stderr, "used blocks: %d\n", used_blocks);

    set_bits(&fs->b_block, 0, used_blocks-1);
    reset_bits(&fs->b_block, used_blocks, fs->disk->nblocks-1);

    reset_bits(&fs->b_inode, 0, NUMBER_OF_INODES-1);

    fs_flush(fs);
}

int  fs_create(file_system_t *fs, int file_type) {
    int new_inode = next_available_block(&fs->b_inode);
    set_bits(&fs->b_inode, new_inode, new_inode);

    int av = next_available_block(&fs->b_block);
    set_bits(&fs->b_block, av, av);

    inode_t inode;
    inode_init(&inode, file_type, 0, av, 0, 0);
    write_inode(fs, new_inode, &inode);
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
    inode_t inode;
    read_inode(fs, inode_index, &inode);

    inode.size = size;
    time(&inode.updated_at);

    uint8_t *pointers = (uint8_t *) malloc(fs->disk->block_size);
    disk_read(fs->disk, pointers, inode.disk_block_ptr);
    
    int needed_blocks = (size + fs->disk->block_size - 1) / fs->disk->block_size;       // teto(size / BLOCK_SIZE)

    for(; inode.allocated_blocks > needed_blocks; --inode.allocated_blocks) {
        int bit;
        if(inode.allocated_blocks > 1) {
            memcpy(&bit, pointers + sizeof(int) * (inode.allocated_blocks - 2), sizeof(int));
        } else {
            bit = inode.block_index;
        }
        fprintf(stderr, "delete bit: %d\n", bit);
        reset_bits(&fs->b_block, bit, bit);
    }

    for(; inode.allocated_blocks < needed_blocks; ++inode.allocated_blocks) {
        int bit;
        bit = next_available_block(&fs->b_block);
        set_bits(&fs->b_block, bit, bit);

        if(inode.allocated_blocks >= 1) {
            memcpy(pointers + sizeof(int) * (inode.allocated_blocks-1), &bit, sizeof(int));
        } else {
            inode.block_index = bit;
        }

        fprintf(stderr, "alloc bit: %d\n", bit);
    }

    assert(inode.allocated_blocks == needed_blocks);

    int block_index = inode.block_index;
    for(int i=0; i < inode.allocated_blocks; ++i) {
        disk_write(fs->disk, data + i*fs->disk->block_size, block_index);
        memcpy(&block_index, pointers + i*sizeof(int), sizeof(int));
    }

    disk_write(fs->disk, pointers, inode.disk_block_ptr);
    free(pointers);
    write_inode(fs, inode_index, &inode);
}

void fs_delete(file_system_t *fs, int inode_index) {
    fprintf(stderr, "fs_delete()\n");
    reset_bits(&fs->b_inode, inode_index, inode_index);

    inode_t inode;
    read_inode(fs, inode_index, &inode);

    uint8_t *pointers = (uint8_t *) malloc(fs->disk->block_size);
    disk_read(fs->disk, pointers, inode.disk_block_ptr);

    for(int i=0; i<inode.allocated_blocks; ++i) {
        int bit;
        if(i == 0) {
            bit = inode.block_index;
        } else {
            memcpy(&bit, pointers + (i-1) * sizeof(int), sizeof(int));
        }
        fprintf(stderr, "delete bit: %d\n", bit);
        reset_bits(&fs->b_block, bit, bit);
    }

    reset_bits(&fs->b_block, inode.disk_block_ptr, inode.disk_block_ptr);
    free(pointers);
}

int  fs_type(file_system_t *fs, int inode_index) {
    inode_t inode;
    read_inode(fs, inode_index, &inode);

    return inode.type;
}