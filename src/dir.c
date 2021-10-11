#include "../lib/dir.h"
#include <string.h>

void dir_init(dir_t *dir) {
    dir->nitems=0;
}

dir_item_t dir_item_create(char *dirname, int inode) {
    dir_item_t item;
    strcpy(item.name, dirname);
    item.inode = inode;

    return item; 
}

void dir_create(file_system_t *fs, int iinode, char *dirname) {
    int in = fs_create(fs, IF_DIR);
    
    dir_t dir;
    
    dir.items[0] = dir_item_create(".", in);
    dir.items[1] = dir_item_create("..", iinode);
    dir.nitems = 2;

    fs_write(fs, in, (uint8_t*) dir.items, dir.nitems*sizeof(dir_item_t));
    
    dir_t dir_current;

    int size = fs_read(fs, iinode, (uint8_t*)dir_current.items);
    dir_current.nitems = size/sizeof(dir_item_t);

    dir_current.items[(dir_current.nitems)++] = dir_item_create(dirname, in);

    fs_write(fs, iinode, (uint8_t*)dir_current.items, dir_current.nitems*sizeof(dir_item_t));

    fs_flush(fs);
}

void dir_rename(file_system_t *fs, int iinode, char *dirname, char *new_dirname) {
    dir_t dir_current;

    int size = fs_read(fs, iinode, (uint8_t*)dir_current.items);
    dir_current.nitems = size/sizeof(dir_item_t);

    for (int i = 0; i < dir_current.nitems; i++) {
        if (!strcmp(dir_current.items[i].name, dirname)) strcpy(dir_current.items[i].name, new_dirname);
    }

    fs_write(fs, iinode, (uint8_t*)dir_current.items, dir_current.nitems*sizeof(dir_item_t));
    fs_flush(fs);
}

void dir_delete_dir(file_system_t *fs, int iinode) {
    dir_item_t items;
    int size = fs_read(fs, iinode, (uint8_t*)items);

    for (int i = 0; i < size/sizeof(dir_item_t); i++) { 
        dir_delete_dir(fs, items[i].inode);
        fs_delete(&fs, dir_current.items[index].inode);
    }
}

void dir_delete(file_system_t *fs, int iinode, char *dirname) {
    dir_t dir_current;

    int size = fs_read(fs, iinode, (uint8_t*)dir_current.items);
    dir_current.nitems = size/sizeof(dir_item_t);

    for (int i = 2; i < dir_current.nitems; i++) {
        if (!strcmp(dir_current.items[i].name, dirname)) {
            dir_delete_dir(fs, dir_current.items[i].inode);
        }
    }

    fs_write(fs, iinode, (uint8_t*)dir_current.items, dir_current.nitems*sizeof(dir_item_t));
    fs_flush(fs);
}

void dir_read(file_system_t *fs, int iinode, char *dirname, dir_t *dir);
void dir_write(file_system_t *fs, int iinode, char *dirname, dir_t *dir);
int  dir_open(file_system_t *fs, int iinode, char *dirname);