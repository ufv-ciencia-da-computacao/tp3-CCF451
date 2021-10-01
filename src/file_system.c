#include <string.h>
#include <stdio.h>
#include "../lib/file_system.h"
#include "../lib/inode.h"
#include "../lib/dir.h"
#include "../lib/inode.h"
#include "../lib/color.h"

#define MAX_INODES      1024
#define MAX_FILE_SIZE   32768
#define BLOCK_SIZE      256
#define NUMBER_BLOCKS   65536
#define MEM_SIZE        (BLOCK_SIZE * NUMBER_BLOCKS)

#define INODES_FILENAME         "files/inodes"
#define PARTITION_FILENAME      "files/partition"

static int next_inode_id = 1;
static int next_block_id = 1;

void fs_init(file_system_t *fs) {
    fs->path_size = 0;
    fs->inode_index = 0;
    fs->f_inode = fopen(INODES_FILENAME, "r+b");
    fs->f_block = fopen(PARTITION_FILENAME, "r+b");
    fread(&fs->inode, sizeof(inode_t), 1, fs->f_inode);
}

void fs_mount(file_system_t *fs) {
    char byte = 0;
    fs->path_size;
    fs->inode_index = 0;
    fs->f_block = fopen(PARTITION_FILENAME, "w+b");
    fs->f_inode = fopen(INODES_FILENAME, "w+b");
    
    /* ---------------- */
    fwrite(&byte, 1, MEM_SIZE, fs->f_block);
    fseek(fs->f_block, 0, SEEK_SET);
    dir_item_t di[2];
    strcpy(di[0].name, ".");
    di[0].inode = 0;
    strcpy(di[1].name, "..");
    di[1].inode = 0;
    fwrite(di, sizeof(dir_item_t), 2, fs->f_block);

    /* ---------------- */
    fwrite(&byte, 1, MAX_INODES * sizeof(inode_t), fs->f_inode);
    fseek(fs->f_inode, 0, SEEK_SET);
    inode_t root_inode;
    inode_init(&root_inode, IF_DIR, 2 * sizeof(dir_item_t), 0, 0, 1);
    
    /* ---------------- */
    fs->inode = root_inode;
    fwrite(&root_inode, sizeof(inode_t), 1, fs->f_inode);
}

void fs_curr_path(file_system_t *fs, char *str) {
    strcpy(str, "/");
    for(int i=0; i<fs->path_size; ++i) {
        strcat(str, fs->path[i].name);
        if(i != fs->path_size-1) strcat(str, "/");
    }
}

void fs_create_dir(file_system_t *fs, char *path) {
    inode_t new_inode;
    inode_init(&new_inode, IF_DIR, 2 * sizeof(dir_item_t), 0, next_block_id * 256, 1);
    
    /**/
    fseek(fs->f_inode, next_inode_id * sizeof(inode_t), SEEK_SET);
    fwrite(&new_inode, sizeof(inode_t), 1, fs->f_inode);

    /**/
    dir_item_t di2[2];
    strcpy(di2[0].name, ".");
    di2[0].inode = next_inode_id;
    strcpy(di2[1].name, "..");
    di2[1].inode = fs->inode_index;
    fseek(fs->f_block, next_block_id * 256, SEEK_SET);
    fwrite(di2, sizeof(dir_item_t), 2, fs->f_block);

    /**/
    fseek(fs->f_block, fs->inode.block_index * 256, SEEK_SET);
    fseek(fs->f_block, fs->inode.size, SEEK_CUR);
    dir_item_t di;
    dir_item_init(&di, path, next_inode_id);
    fwrite(&di, sizeof(dir_item_t), 1, fs->f_block);

    fs->inode.size += sizeof(dir_item_t);
    fseek(fs->f_inode, fs->inode_index * sizeof(inode_t), SEEK_SET);
    fwrite(&fs->inode, sizeof(inode_t), 1, fs->f_inode);

    next_block_id++;
    next_inode_id++;
    fflush(fs->f_inode);
    fflush(fs->f_block);
}

void fs_list_dir(file_system_t *fs) {
    for(int i = 2 * sizeof(dir_item_t); i < fs->inode.size; i += sizeof(dir_item_t)) {
        dir_item_t di;
        fseek(fs->f_block, i, SEEK_SET);
        fread(&di, sizeof(dir_item_t), 1, fs->f_block);

        // printf("%s | %d\n", di.name, di.inode);
        
        inode_t inode;
        fseek(fs->f_inode, di.inode * sizeof(inode_t), SEEK_SET);
        fread(&inode, sizeof(inode_t), 1, fs->f_inode);
        
        if(inode.type == IF_DIR) {
            color_blue(di.name);
        } else {
            color_white(di.name);
        }
        printf("\n");
    }
}
