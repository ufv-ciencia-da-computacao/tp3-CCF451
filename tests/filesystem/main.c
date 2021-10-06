#include <stdio.h>
#include "../../lib/file_system.h"

int main() {

    disk_t disk;
    disk_open(&disk, "../../files/partition");
    // disk_format(&disk, 1<<9, 1<<12);
    disk_mount(&disk);

    file_system_t fs;
    fs_init(&fs, &disk);
    //fs_format(&fs);



    return 0;
}