#include <stdio.h>
#include "../../lib/file.h"

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
    file_t file;

    dir.items[0] = dir_item_create(".", inode);
    dir.items[1] = dir_item_create("..", inode);
    dir.nitems = 2;

    fs_write(&fs, inode, (uint8_t*)dir.items, dir.nitems*sizeof(dir_item_t));

    dir_create(&fs, inode, "cdc");
    dir_create(&fs, inode, "cdc1");
    dir_create(&fs, inode, "cdc2");
    dir_create(&fs, inode, "cdc3");
    dir_create(&fs, inode, "cdc4");
    dir_create(&fs, inode, "cdc5");
    file_create(&fs,0,"file1");//ok
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


    //file_rename(&fs, 7, "file1", "file2");//ok
    //dir_delete(&fs, 7, "file2");//ok

    file_t file_r;
    file_t file_w;

    //strcpy(file_w.data,"dener é um viadinho e germano um chifrudinho");
    //file_w.size=strlen(file_w.data)+1;

    //file_write(&fs,7,"file2",&file_w);
    //file_read(&fs,7,"file2",&file_r);

    //printf("dados:%s\n",file_r.data);


    dir_t teste;
    dir_read(&fs,2,&teste);
    printf("%d\n",teste.nitems );
    file_move(&fs,0,"file1","../cdc1/caf/file1");


    size = fs_read(&fs, 0, (uint8_t*)dir_current.items);

    dir_current.nitems = size/sizeof(dir_item_t);

    for (int i = 0; i < dir_current.nitems; i++) {
       printf("%s %d\t", dir_current.items[i].name, dir_current.items[i].inode);
    }

    printf("\n");

    size = fs_read(&fs, 2, (uint8_t*)dir_current.items);

   dir_current.nitems = size/sizeof(dir_item_t);

   for (int i = 0; i < dir_current.nitems; i++) {
       printf("%s %d\t", dir_current.items[i].name, dir_current.items[i].inode);
   }

   printf("\n");
    size = fs_read(&fs, 8, (uint8_t*)dir_current.items);

   dir_current.nitems = size/sizeof(dir_item_t);

   for (int i = 0; i < dir_current.nitems; i++) {
       printf("%s %d\t", dir_current.items[i].name, dir_current.items[i].inode);
   }

   printf("\n");


    //
    // size = fs_read(&fs, subsubinode, (uint8_t*)dir_current.items);
    //
    // dir_current.nitems = size/sizeof(dir_item_t);
    //
    // for (int i = 0; i < dir_current.nitems; i++) {
    //     printf("%s %d\t", dir_current.items[i].name, dir_current.items[i].inode);
    // }
    //
    // printf("\n");
    //
    //
    // // printf("\n");
    // //
    // // dir_delete(&fs, 2, "caf");
    // //
    // // inode = 2;
    // //
    // // size = fs_read(&fs, inode, (uint8_t*)dir_current.items);
    // // dir_current.nitems = size/sizeof(dir_item_t);
    // //
    // // for (int i = 0; i < dir_current.nitems; i++) {
    // //     printf("%s %d\t", dir_current.items[i].name, dir_current.items[i].inode);
    // // }
    // //
    // // printf("\n");

    fs_flush(&fs);
    fs_destroy(&fs);
    disk_close(&disk);

    return 0;
}
