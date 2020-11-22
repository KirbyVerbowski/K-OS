#ifndef HEADER_io
#define HEADER_io

/*Write value to I/O port
 *Implemented in io.s
 */
extern void set_io(unsigned short port, unsigned char value);

/*Read value from I/O port
 *Implemented in io.s
 */
extern unsigned char get_io(unsigned short port);

#endif