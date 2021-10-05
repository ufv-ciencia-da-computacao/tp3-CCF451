#include "../../lib/disk.h"

int main() {

    disk_t disk;
    disk_open(&disk, "../../files/partition");
    if(disk.file == NULL) {
        printf("error\n");
        return 0;
    }
    disk_format(&disk, 1<<10, 1<<14);
    disk_mount(&disk);

    printf("%d %d %d\n", disk.block_size, disk.nblocks, disk.size);

    return 0;
}