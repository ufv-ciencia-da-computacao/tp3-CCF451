.PHONY: all
all: main

main: color cmd_if file_system inode dir file bitmap disk
	gcc main.c color.o cmd_if.o file_system.o inode.o dir.o file.o bitmap.o disk.o -o main
	rm *.o

color:
	gcc -c ./src/color.c -o color.o

cmd_if:
	gcc -c ./src/cmd_if.c -o cmd_if.o

file_system:
	gcc -c ./src/file_system.c -o file_system.o

inode:
	gcc -c ./src/inode.c -o inode.o

dir:
	gcc -c ./src/dir.c -o dir.o

disk:
	gcc -c ./src/disk.c -o disk.o

file:
	gcc -c ./src/file.c -o file.o

bitmap:
	gcc -c ./src/bitmap.c -o bitmap.o

clean:
ifeq ($(OS),Windows_NT)
	del main.exe
else
	rm main
endif
