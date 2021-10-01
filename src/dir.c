#include "../lib/dir.h"
#include <string.h>

void dir_item_init(dir_item_t *di, char name, int inode) {
    strcpy(di->name, name);
    di->inode = inode;
}