#! /bin/bash
make compiler
#read -p "waiting for enter"
./compiler <test.c >tmp.s
nasm -felf -o tmp.o tmp.s
gcc -m32 -o tmp.out tmp.o
./tmp.out hello
echo $?

