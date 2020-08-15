#! /bin/bash 
cpp $1 -nostdinc -I./include/ -I./hdr -o pp.c -E
cat pp.c | ./compiler > tmp.s
nasm -felf -g -F dwarf -o tmp.o tmp.s
cp "./tmp.o" "./$2"
rm ./tmp.o
rm ./tmp.s

