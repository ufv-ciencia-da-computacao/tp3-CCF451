.PHONY: all

all: main clean

main: color commands
	gcc main.c color.o commands.o -o main

color:
	gcc -c ./src/color.c -o color.o

commands:
	gcc -c ./src/commands.c -o commands.o

clean:
ifeq ($(OS),Windows_NT)
#	del main
	del *.o
else
	rm -rf *.o
endif
