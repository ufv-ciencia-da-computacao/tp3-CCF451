.PHONY: all
all: main

mount: color cmd_if file_system inode
	gcc -D MOUNT main.c color.o cmd_if.o file_system.o inode.o -o main
	rm *.o

main: color cmd_if file_system inode
	gcc main.c color.o cmd_if.o file_system.o inode.o -o main
	rm *.o

color:
	gcc -c ./src/color.c -o color.o

cmd_if:
	gcc -c ./src/cmd_if.c -o cmd_if.o

file_system:
	gcc -c ./src/file_system.c -o file_system.o

inode:
	gcc -c ./src/inode.c -o inode.o

clean:
ifeq ($(OS),Windows_NT)
#	del main
	del *.o
else
	rm main
endif
