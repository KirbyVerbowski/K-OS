#ifndef HEADER_keyboard
#define HEADER_keyboard

int configure_keyboard();

unsigned char read_scancode();

unsigned char read_ASCII();

unsigned char scancode2ASCII(unsigned char scancode);

#endif