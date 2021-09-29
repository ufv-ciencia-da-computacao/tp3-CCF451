.PHONY: all

all: main

main:
	gcc main.c -o main

clean:
ifeq ($(OS),Windows_NT)
#	del main
	del *.o
else
	rm -rf *.o
endif
