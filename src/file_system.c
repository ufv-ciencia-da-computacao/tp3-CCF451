#include <string.h>
#include <stdio.h>
#include "../lib/file_system.h"
#include "../lib/inode.h"
#include "../lib/dir.h"
#include "../lib/inode.h"

#define MAX_INODES      1024
#define MAX_FILE_SIZE   32768
#define BLOCK_SIZE      256
#define NUMBER_BLOCKS   65536
#define MEM_SIZE        (BLOCK_SIZE * NUMBER_BLOCKS)

void fs_init(file_system_t *fs) {
    fs->path_size = 0;
    fs->inode_index = 0;
    FILE *f = fopen("files/inodes", "rb");
    fread(&fs->inode, sizeof(inode_t), 1, f);
    fclose(f);
}

void fs_mount(file_system_t *fs) {
    char byte = 0;
    fs->path_size;
    fs->inode_index = 0;
    
    /* ---------------- */
    FILE *f_partition = fopen("files/partition", "w+b");
    fwrite(&byte, 1, MEM_SIZE, f_partition);
    fseek(f_partition, 0, SEEK_SET);
    dir_item_t di[2];
    strcpy(di[0].name, ".");
    di[0].inode = 0;
    strcpy(di[0].name, "..");
    di[0].inode = 0;
    fwrite(di, sizeof(dir_item_t), 2, f_partition);
    fclose(f_partition);

    /* ---------------- */
    FILE *f_inodes = fopen("files/inodes", "w+b");
    fwrite(&byte, 1, MAX_INODES * sizeof(inode_t), f_inodes);
    fseek(f_inodes, 0, SEEK_SET);
    inode_t root_inode;
    inode_init(&root_inode, IF_DIR, 2 * sizeof(dir_item_t), 0, 0, 1);
    fs->inode = root_inode;
    fwrite(&root_inode, sizeof(inode_t), 1, f_inodes);
    fclose(f_inodes);
}

void fs_curr_path(file_system_t *fs, char str[]) {
    strcpy(str, "/");
    for(int i=0; i<fs->path_size; ++i) {
        strcat(str, fs->path[i].name);
        if(i != fs->path_size-1) strcat(str, "/");
    }
}

void fs_create_dir(file_system_t *fs) {

}

void fs_create_file() {

}