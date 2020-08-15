#! /bin/bash
make compiler
#read -p "waiting for enter"
cpp test.c -nostdinc -I./include/ -I./hdr -o pp.c -E
./compiler <pp.c >tmp.s
nasm -felf -g -F dwarf -o tmp.o tmp.s
gcc -m32 -o tmp.out tmp.o
./tmp.out hello
echo $?

