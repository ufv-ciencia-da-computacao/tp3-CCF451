#include "../../lib/disk.h"

int main() {

    disk_t disk;
    disk_open(&disk, "../../files/partition");
    if(disk.file == NULL) {
        printf("error\n");
        return 0;
    }
    /*
        disk format are
        bsize   nblocks inodes
        512     4096    128
        4096    4096    1024
    */
    disk_format(&disk, 1<<9, 1<<12);
    disk_mount(&disk);

    char write[4096];
    char read[4096];

    sprintf(write, "Hello World\n");
    disk_write(&disk, write, 1);

    disk_read(&disk, read, 1);

    printf("%d %d %d\n", disk.block_size, disk.nblocks, disk.size);
    printf("%s", read);

    return 0;
}