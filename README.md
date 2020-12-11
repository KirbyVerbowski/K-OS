# K-OS

32-bit operating system targeting x86 archetecture

To compile, link, and generate a bootable ISO: (Assuming you have the dependencies installed - NASM, genisoimage, gcc, ld)
 `make all`
 
To do above and run (Above dependencies + Bochs)
 `make run`


Assembly is written in Intel syntax for the NASM compiler producing 32-bit ELF.

C is targetting GCC.

OS will be loaded by GRUB legacy (because I don't want to write a bootloader)
motivating most of the above choices.

11/19/2020

This project is in it's very early stages.
I'm using this to learn more about how the operating system is loaded,
creating a usable memory space for C, and writing x86 assembly.
And obviously I'm trying to learn about the specific implementation of
core OS features (scheduler, virtual memory, filesystem, etc.)
that I learned about at a high level in class.

12/11/2020

The bootstrapping process is nearly complete. Features include:
- Higher-half kernel (loaded at paddr:0x00000000 vaddr:0xC0000000)
- Basic Keyboard input
- Output to the VGA display
- Paging/segmentation allowing for readonly and user level pages
- Interrupt handler which handles page faults and keyboard interrupts (all others cause a debug breakpoint)
- Dynamic page frame allocation
- A very simple linkedlist implementation of malloc() and free()

Next up:
- Method for swapping pages on task switch
- Create a PCB struct
- Configure serial port(s) to stream binary data in
- Dynamically load and run a user level C program cross compiled to ELF binary
- Make a scheduler 
- Write a shell
- Filesystem
