#ifndef __DISK_H__
#define __DISK_H__

#include <stdio.h>
#include <stdint.h>

typedef struct {
    FILE *file;
    int block_size;
    int nblocks;
    int size;
} disk_t;

void disk_open(disk_t *disk, char *filename);
void disk_close(disk_t *disk);
void disk_mount(disk_t *disk);
void disk_format(disk_t *disk, int block_size, int nblocks);

void disk_read(disk_t *disk, uint8_t *data, uint16_t block_index);
void disk_write(disk_t *disk, uint8_t *data, uint16_t block_index);

#endif