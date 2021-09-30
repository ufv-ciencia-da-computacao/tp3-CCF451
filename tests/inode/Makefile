.PHONY: all

all: main clean

main: inode.o main.o
	gcc inode.o main.o -o main

main.o: ./main.c
	gcc -c ./main.c -o main.o

inode.o: ../../src/inode.c
	gcc -c ../../src/inode.c -o inode.o

clean:
ifeq ($(OS),Windows_NT)
#	del main
	del *.o
else
	rm -rf *.o
endif