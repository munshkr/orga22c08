@echo off
cls
yasm -fwin32 -o dividir.o bmp2joc2/dividir.asm
gcc -c bmp2joc2/bmp2joc2.c
gcc -o bmp2joc2.exe bmp2joc2.o dividir.o -lm
del *.o
