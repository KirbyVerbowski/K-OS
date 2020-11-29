#ifndef HEADER_kernel
#define HEADER_kernel

//KeyEvent struct
#define KEYBUF 0x00200008;
/** Buffer of held modifier keys:
  * Bit  |   7    |6|  5  |  4  |  3 |  2 |  1 |  0
  *       Pressed? 0 LCTRL RCTRL LSFT RSFT LALT RALT
  */
#define KEYMOD 0x0020000E;

#endif