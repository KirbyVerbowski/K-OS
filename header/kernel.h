#ifndef HEADER_kernel
#define HEADER_kernel

#include "keyboard.h"

#define KERNEL_OFFSET 	0xC0000000
#define KERNEL_SIZE 	0x00400000

extern KeyEvent * keyboard_buffer;
extern char * keyboard_modifiers;

#endif