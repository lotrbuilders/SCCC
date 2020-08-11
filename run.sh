#! /bin/bash
make compiler
./compiler <test.c >tmp.s
nasm -felf -o tmp.o tmp.s
gcc -m32 -o tmp.out tmp.o
./tmp.out
echo $?

