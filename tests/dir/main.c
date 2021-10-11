#include <stdio.h>
#include "../../lib/dir.h"

int main() {

    disk_t disk;
    disk_open(&disk, "../../files/partition");
    disk_mount(&disk);

    file_system_t fs;
    fs_init(&fs, &disk);
    
    disk_format(&disk, 1<<9, 1<<12); fs_format(&fs);

    int N = 5;
    char string[MAX_FILE_SIZE] = "Testando 1 2 3 4 5 6 7 8 9\n";

    int inode = fs_create(&fs, IF_DIR);
    dir_t dir;
    
    dir.items[0] = dir_item_create(".", inode);
    dir.items[1] = dir_item_create("..", inode);
    dir.nitems = 2;

    fs_write(&fs, inode, (uint8_t*)dir.items, dir.nitems*sizeof(dir_item_t));

    dir_create(&fs, inode, "cdc");
    dir_create(&fs, inode, "cdc");
    dir_create(&fs, inode, "cdc");
    dir_create(&fs, inode, "cdc");
    dir_create(&fs, inode, "cdc");
    dir_create(&fs, inode, "cdc");

    dir_t dir_current;
    int size = fs_read(&fs, inode, (uint8_t*)dir_current.items);

    dir_current.nitems = size/sizeof(dir_item_t);

    for (int i = 0; i < dir_current.nitems; i++) {
        printf("%s %d\t", dir_current.items[i].name, dir_current.items[i].inode);
    }

    printf("\n");
    
    int subinode = 2;
    dir_create(&fs, subinode, "caf");

    size = fs_read(&fs, subinode, (uint8_t*)dir_current.items);

    dir_current.nitems = size/sizeof(dir_item_t);

    for (int i = 0; i < dir_current.nitems; i++) {
        printf("%s %d\t", dir_current.items[i].name, dir_current.items[i].inode);
    }

    printf("\n");

    int subsubinode = 7;
    dir_create(&fs, subsubinode, "carlinhos");

    size = fs_read(&fs, subsubinode, (uint8_t*)dir_current.items);

    dir_current.nitems = size/sizeof(dir_item_t);

    for (int i = 0; i < dir_current.nitems; i++) {
        printf("%s %d\t", dir_current.items[i].name, dir_current.items[i].inode);
    }

    printf("\n");

    dir_rename(&fs, 7, "carlinhos", "caca");

    subsubinode = 7;

    size = fs_read(&fs, subsubinode, (uint8_t*)dir_current.items);
    dir_current.nitems = size/sizeof(dir_item_t);

    for (int i = 0; i < dir_current.nitems; i++) {
        printf("%s %d\t", dir_current.items[i].name, dir_current.items[i].inode);
    }

    printf("\n");

    fs_flush(&fs);
    fs_destroy(&fs);
    disk_close(&disk);

    return 0;
}