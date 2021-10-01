.PHONY: all

all: main clean

main: color cmd_if
	gcc main.c color.o cmd_if.o -o main

color:
	gcc -c ./src/color.c -o color.o

cmd_if:
	gcc -c ./src/cmd_if.c -o cmd_if.o

clean:
ifeq ($(OS),Windows_NT)
#	del main
#	del *.o
else
	rm -rf *.o
endif
