#! /bin/bash
cat $1 | ../compiler > tmp.s
nasm -felf -o tmp.o tmp.s
gcc -m32 -o tmp.out tmp.o
rm tmp.s 
rm tmp.o


