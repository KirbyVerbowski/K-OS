#ifndef HEADER_kernel
#define HEADER_kernel

#include "keyboard.h"

#define KERNEL_OFFSET 0xC0000000

extern KeyEvent * keyboard_buffer;
extern char * keyboard_modifiers;

#endif