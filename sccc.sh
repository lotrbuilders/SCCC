#! /bin/bash
cat $1 | ./compiler > tmp.s
nasm -felf -o tmp.o tmp.s

