#!/bin/bash

gcc main.c ../../src/disk.c ../../src/file_system.c ../../src/bitmap.c ../../src/inode.c -o main
./main 2>err