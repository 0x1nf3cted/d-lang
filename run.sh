#!/bin/sh

cd build && cmake .. && make && ./dzair ../vars.dz && sleep 1


# for AT&T syntax
# as -o vars.o vars.asm
# ld -o vars vars.o


nasm -f elf vars.asm -o vars.o
ld -m elf_i386 vars.o -o vars


rm vars.o
rm vars.asm

cd ..

mv build/vars .

./vars