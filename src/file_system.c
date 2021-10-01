#include <string.h>
#include "../lib/file_system.h"
#include "../lib/inode.h"
#include "../lib/dir.h"

#define MAX_INODES      1024
#define MAX_FILE_SIZE   32768
#define BLOCK_SIZE      256
#define NUMBER_BLOCKS   65536

dir_item_t path[200];
int path_size = 0;

void fs_curr_path(char str[]) {
    strcpy(str, "/");
    for(int i=0; i<path_size; ++i) {
        strcat(str, path[i].name);
        if(i != path_size-1) strcat(str, "/");
    }
}