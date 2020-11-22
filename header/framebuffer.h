#ifndef HEADER_framebuffer
#define HEADER_framebuffer

#define FB_BLACK		0
#define FB_BLUE			1
#define FB_GREEN		2
#define FB_CYAN			3
#define FB_RED			4
#define FB_MAGENTA		5
#define FB_BROWN		6
#define FB_LIGHTGREY	7
#define FB_DARKGREY		8
#define FB_LIGHTBLUE	9
#define FB_LIGHTGREEN	10
#define FB_LIGHTCYAN	11
#define FB_LIGHTRED		12
#define FB_LIGHTMAGENTA	13
#define FB_LIGHTBROWN	14
#define FB_WHITE		15

#define FB_COLS	80
#define FB_ROWS 25

#define FB_DATA_PORT    0x03D5
#define FB_DESC_PORT	0x03D4
#define FB_CURS_POS_H	0x0E
#define FB_CURS_POS_L   0x0F

/*Write value to I/O port
 *Implemented in kstdio_s.s
 */
extern void set_io(unsigned short port, unsigned char value);

/*Read value from I/O port
 *Implemented in kstdio_s.s
 */
extern unsigned char get_io(unsigned short port);

/*Write the specified character to the framebuffer cell at position (x, y) using specified colors
 *Implemented in kstdio_s.s
 *Returns: 0 if successful, 1 if x out of bounds, 2 if y is out of bounds
 */
extern int write_fb_cell(unsigned short x, unsigned short y, char character, unsigned char bg, unsigned char fg);

/*Move the framebuffer cursor to the specified position (x, y)
 *Returns: 0 if successful, 1 if x out of bounds, 2 if y is out of bounds
 */
int set_fb_cursor(unsigned char x, unsigned char y);

/*Get the framebuffer curor position ( = y * FB_COLS + x)
 */
unsigned short get_fb_cursor(void);

/*Write c to the framebuffer at the current cursor position, and advance the cursor
 */
void putch(unsigned char c);

/*Write string to framebuffer at current cursor position. Cursor is advanced to cell after last character
 */
void puts(char* c);

/*Like puts but for an int. 'format' dictates how the int is displayed. (d=decimal, x=hex, b=binary. Prefix with 0 for leading zeros)
 */
void putint(unsigned int x, char* format);

#endif
