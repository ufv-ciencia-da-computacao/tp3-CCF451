#include "../lib/file.h"

void file_create(file_system_t *fs, int iinode, char *filename) {
	int inodeFile = fs_create(fs, IF_FILE);

	dir_t dir_current;

	int size = fs_read(fs, iinode, (uint8_t *)dir_current.items);
	dir_current.nitems = size / sizeof(dir_item_t);

	dir_current.items[(dir_current.nitems)++] = dir_item_create(filename, inodeFile);

	fs_write(fs, iinode, (uint8_t *)dir_current.items, dir_current.nitems * sizeof(dir_item_t));

	fs_flush(fs);
}
void file_rename(file_system_t *fs, int iinode, char *filename, char *new_filename) {
	dir_t dir_current;

	int size = fs_read(fs, iinode, (uint8_t *)dir_current.items);
	dir_current.nitems = size / sizeof(dir_item_t);

	for (int i = 0; i < dir_current.nitems; i++) {
		if (!strcmp(dir_current.items[i].name, filename))
			strcpy(dir_current.items[i].name, new_filename);
	}

	fs_write(fs, iinode, (uint8_t *)dir_current.items, dir_current.nitems * sizeof(dir_item_t));
	fs_flush(fs);
}
void file_move(file_system_t *fs, int iinode, char *filename, char *path) {
	file_t curr_file;
	int inodePath=(path[0]=='/'?0:iinode);
	file_read(fs,iinode,filename,&curr_file);
	char str[200];
	int posicao=0;
	dir_t dir;
	for(int i=(path[0]=='/'?1:0);i<(int)strlen(path);i++){

		if(path[i]=='/'){
			//printf("%s %d\n",str,inodePath );

			fflush(stdout);
			dir_read(fs,inodePath,&dir);
			for(int j=0;j<dir.nitems;j++){
				if(strcmp(str,dir.items[j].name)==0){
					inodePath=dir.items[j].inode;
					break;
				}
			}

			posicao=0;

		}else{
			str[posicao++]=path[i];
			str[posicao]='\0';
		}
	}
	file_create(fs,inodePath,str);
	file_write(fs, inodePath, str, &curr_file);
	file_delete(fs,iinode,filename);
}
void file_delete(file_system_t *fs, int iinode, char *filename) {
	dir_t dir_current;
	dir_read(fs,iinode,&dir_current);
	for (int i = 2; i < dir_current.nitems;) {
		if (strcmp(dir_current.items[i].name, filename) == 0) {
			fs_delete(fs, dir_current.items[i].inode);

			for (int j = i + 1; j < dir_current.nitems; j++) copy_dir_item(&(dir_current.items[j]), &(dir_current.items[j - 1]));

			dir_current.nitems--;
		}else{
			i++;
		}
	}

	fs_write(fs, iinode, (uint8_t *)dir_current.items, dir_current.nitems * sizeof(dir_item_t));
	fs_flush(fs);
}

void file_read(file_system_t *fs, int iinode, char *filename, file_t *file){
  dir_t dir_current;
	int size = fs_read(fs, iinode, (uint8_t *)dir_current.items);
	for (int i = 2; i < size / sizeof(dir_item_t); i++) {
		if (strcmp(dir_current.items[i].name, filename) == 0) {
			file->size=fs_read(fs, dir_current.items[i].inode, (uint8_t *)file->data);
      break;
		}
	}
}
void file_write(file_system_t *fs, int iinode, char *filename, file_t *file){
  dir_t dir_current;
  int size = fs_read(fs, iinode, (uint8_t *)dir_current.items);
  for (int i = 2; i < size / sizeof(dir_item_t); i++) {
    if (strcmp(dir_current.items[i].name, filename) == 0) {
      fs_write(fs, dir_current.items[i].inode, (uint8_t *)file->data,file->size);
			fs_flush(fs);
      break;
    }
  }
}
