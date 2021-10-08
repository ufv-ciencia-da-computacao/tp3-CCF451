#include <stdio.h>
#include "../../lib/file_system.h"

int main() {

    disk_t disk;
    disk_open(&disk, "../../files/partition");
    disk_mount(&disk);

    file_system_t fs;
    fs_init(&fs, &disk);
    
    // disk_format(&disk, 1<<9, 1<<12); fs_format(&fs);

    int N = 5;
    char string[MAX_FILE_SIZE] = "Testando 1 2 3 4 5 6 7 8 9\n";
    int iinode = fs_create(&fs, IF_FILE);
    // int iinode = 0;
    fprintf(stderr, "iinode: %d\n", iinode);

    fs_write(&fs, iinode, string, 50);

    // fprintf(stderr, "new file: %d\n", iinode);
    // fs_write(&fs, iinode, string, 1000);
    // fprintf(stderr, "\n");
    // fs_write(&fs, iinode, string, 500);
    // fprintf(stderr, "\n");
    // fs_write(&fs, iinode, string, 4096);
    // fprintf(stderr, "\n");
    // fs_write(&fs, iinode, string, 1023);
    // fprintf(stderr, "\n");
    // fs_write(&fs, iinode, string, 1025);
    // fprintf(stderr, "\n");

    int size = fs_read(&fs, iinode, string);
    fprintf(stderr, "read: %d %s\n", size, string);

    fs_delete(&fs, iinode);

    // for(int i=0; i<N; ++i) {
    //     sfprintf(stderr, string, "Hello from inode %d\n", inodes[i]);
    //     fs_write(&fs, inodes[i], string, 1000);
    // }

    // for(int i=0; i<N; ++i) {
    //     int size = fs_read(&fs, inodes[i], string);
    //     fprintf(stderr, "read size: %d\nread text: %s", size, string);
    // }

    fs_flush(&fs);
    fs_destroy(&fs);
    disk_close(&disk);

    return 0;
}