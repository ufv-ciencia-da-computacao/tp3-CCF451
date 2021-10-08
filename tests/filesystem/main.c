#include <stdio.h>
#include "../../lib/file_system.h"

int main() {

    disk_t disk;
    disk_open(&disk, "../../files/partition");
    // disk_format(&disk, 1<<9, 1<<12);
    disk_mount(&disk);

    file_system_t fs;
    fs_init(&fs, &disk);
    // fs_format(&fs);
    
    int iinode = fs_create(&fs, IF_FILE);
    printf("index_inode = %d\n", iinode);

    // iinode = fs_create(&fs, IF_FILE);
    // printf("index_inode = %d\n", iinode);

    // iinode = fs_create(&fs, IF_FILE);
    // printf("index_inode = %d\n", iinode);



    fs_flush(&fs);
    // fs_destroy(&fs);
    // disk_close(&disk);

    return 0;
}