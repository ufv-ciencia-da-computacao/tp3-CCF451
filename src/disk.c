#include "../lib/disk.h"

void disk_open(disk_t *disk, char *filename) {
    disk->file = fopen(filename, "r+b");
    disk->block_size = 0;
    disk->nblocks = 0;
    disk->size = 0;
}

void disk_close(disk_t *disk) {
    fclose(disk->file);
}

void disk_mount(disk_t *disk) {
    fseek(disk->file, 0, SEEK_SET);
    fread(&disk->block_size, sizeof(int), 1, disk->file);
    fread(&disk->nblocks, sizeof(int), 1, disk->file);
    disk->size = disk->block_size * disk->nblocks;
}

void disk_format(disk_t *disk, int block_size, int nblocks) {
    uint8_t byte = 0;
    for(int i=0; i<block_size * nblocks; ++i) fwrite(&byte, 1, 1, disk->file);
    fseek(disk->file, 0, SEEK_SET);
    fwrite(&block_size, sizeof(int), 1, disk->file);
    fwrite(&nblocks, sizeof(int), 1, disk->file);
    fflush(disk->file);
}

void disk_read(disk_t *disk, uint8_t *data, int block_index) {
    fseek(disk->file, block_index * disk->block_size, SEEK_SET);
    fread(data, sizeof(uint8_t), disk->block_size, disk->file);
}

void disk_write(disk_t *disk, uint8_t *data, int block_index) {
    fseek(disk->file, block_index * disk->block_size, SEEK_SET);
    fwrite(data, sizeof(uint8_t), disk->block_size, disk->file);
    fflush(disk->file);
}