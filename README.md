# K-OS

32-bit operating system targeting x86 archetecture

To compile, link, and generate a bootable ISO: (Assuming you have the dependencies installed - NASM, genisoimage, gcc, ld)
 `make all`
 
To do above and run (Above dependencies + Bochs)
 `make run`


Assembly is written in Intel syntax for the NASM compiler producing 32-bit ELF
C is targetting GCC 
OS will be loaded by GRUB legacy (because I don't want to write a bootloader)
motivating most of the above choices

11/19/2020

This project is in it's very early stages.
I'm using this to learn more about how the operating system is loaded,
creating a usable memory space for C, and writing x86 assembly.
And obviously I'm trying to learn about the specific implementation of
core OS features (scheduler, virtual memory, filesystem, etc.)
that I learned about at a high level in class.
