#!/bin/bash
arm-none-eabi-gcc -mcpu=arm7tdmi -c main.c
arm-none-eabi-gcc -mcpu=arm7tdmi -c scheduler.c
arm-none-eabi-gcc -mcpu=arm7tdmi -c util.c
arm-none-eabi-gcc -mcpu=arm7tdmi -c interrupt.c
arm-none-eabi-gcc -mcpu=arm7tdmi -c breakpoint.c
arm-none-eabi-gcc -mcpu=arm7tdmi -c tlb.c
arm-none-eabi-gcc -mcpu=arm7tdmi -c interrupt.c
arm-none-eabi-gcc -mcpu=arm7tdmi -c asl.c
arm-none-eabi-gcc -mcpu=arm7tdmi -c pcb.c
arm-none-eabi-gcc -mcpu=arm7tdmi -c test.c
arm-none-eabi-gcc -mcpu=arm7tdmi -c trap.c

arm-none-eabi-ld -T /usr/include/uarm/ldscripts/elf32ltsarm.h.uarmcore.x -o p1test /usr/include/uarm/crtso.o /usr/include/uarm/libuarm.o main.o asl.o pcb.o scheduler.o tlb.o util.o trap.o test.o interrupt.o breakpoint.o

elf2uarm -k p1test

rm *.o
