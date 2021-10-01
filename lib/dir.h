#ifndef __DIR_H__
#define __DIR_H__

typedef struct dir_item {
    char name[28];
    int inode;
} dir_item_t;

void dir_item_init(dir_item_t *di, char name, int inode);

#endif