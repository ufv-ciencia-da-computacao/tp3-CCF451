#include "../lib/file.h"

void file_create(file_system_t *fs, int iinode, char *filename){
  int inodeFile = fs_create(fs, IF_FILE);

  dir_t dir_current;

  int size = fs_read(fs, iinode, (uint8_t*)dir_current.items);
  dir_current.nitems = size/sizeof(dir_item_t);

  dir_current.items[(dir_current.nitems)++] = dir_item_create(filename, inodeFile);

  fs_write(fs, iinode, (uint8_t*)dir_current.items, dir_current.nitems*sizeof(dir_item_t));

  fs_flush(fs);

}
void file_rename(file_system_t *fs, int iinode, char *filename, char *new_filename){
  dir_t dir_current;

  int size = fs_read(fs, iinode, (uint8_t*)dir_current.items);
  dir_current.nitems = size/sizeof(dir_item_t);

  for (int i = 0; i < dir_current.nitems; i++) {
      if (!strcmp(dir_current.items[i].name, filename)) strcpy(dir_current.items[i].name, new_filename);
  }

  fs_write(fs, iinode, (uint8_t*)dir_current.items, dir_current.nitems*sizeof(dir_item_t));
  fs_flush(fs);
}
void file_move(file_system_t *fs, int iinode, char *filename, char *path){
  // novo arq
  // copiar dados no novo arq
  // apaga antigo arq
}
void file_delete(file_system_t *fs, int iinode, char *filename){
  dir_t dir_current;
  int size = fs_read(fs, iinode, (uint8_t*)dir_current.items);
  int contador=0;
  for (int i = 2; i < size/sizeof(dir_item_t); i++) {
      if (strcmp(dir_current.items[i].name,filename)==0) {
        fs_delete(fs, dir_current.items[i].inode);
    }
      printf("%d\n", dir_current.items[i].inode);
  }

  for (int i = 2; i < dir_current.nitems; i++) {
      if (!strcmp(dir_current.items[i].name, dirname)) {
          for (int j = i+1; j < dir_current.nitems; j++) copy_dir_item(&(dir_current.items[j]), &(dir_current.items[j-1]));
          dir_current.nitems--;
      }
  }

  fs_write(fs, iinode, (uint8_t*)dir_current.items, dir_current.nitems*sizeof(dir_item_t));
  fs_flush(fs);

}
void file_read(file_system_t *fs, int iinode, char *filename, file_t *file){
  
}
void file_write(file_system_t *fs, int iinode, char *filename, file_t *file){

}
