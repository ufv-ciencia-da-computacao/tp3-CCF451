#include <stdio.h>
#include "../../lib/file_system.h"

int main() {

    disk_t disk;
    disk_open(&disk, "../../files/partition");
    disk_mount(&disk);

    file_system_t fs;
    fs_init(&fs, &disk);
    
    disk_format(&disk, 1<<9, 1<<12); fs_format(&fs);

    int N = 5;
    char string[MAX_FILE_SIZE] = "Hello World\n";
    int iinode = fs_create(&fs, IF_FILE);

    printf("new file: %d\n", iinode);
    fs_write(&fs, iinode, string, 1000);
    printf("\n");
    fs_write(&fs, iinode, string, 500);
    printf("\n");
    fs_write(&fs, iinode, string, 4096);
    printf("\n");
    fs_write(&fs, iinode, string, 1023);
    printf("\n");
    fs_write(&fs, iinode, string, 1025);
    printf("\n");
    fs_delete(&fs, iinode);

    // for(int i=0; i<N; ++i) {
    //     sprintf(string, "Hello from inode %d\n", inodes[i]);
    //     fs_write(&fs, inodes[i], string, 1000);
    // }

    // for(int i=0; i<N; ++i) {
    //     int size = fs_read(&fs, inodes[i], string);
    //     printf("read size: %d\nread text: %s", size, string);
    // }

    fs_flush(&fs);
    fs_destroy(&fs);
    disk_close(&disk);

    return 0;
}