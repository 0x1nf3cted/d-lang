#!/bin/sh

cd build && cmake .. && make && ./dzair ../vars.dz && sleep 1

as -o vars.o vars.asm
ld -o vars vars.o

rm vars.o
rm vars.asm

cd ..

mv build/vars .