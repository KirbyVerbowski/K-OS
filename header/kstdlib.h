#ifndef HEADER_kstdlib
#define HEADER_kstdlib

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

/*Write the specified character to the framebuffer cell at position (x, y) using specified colors
 *Implemented in kstdlib_s.s
 *Returns: 0 if successful, 1 if x out of bounds, 2 if y is out of bounds
 */
extern int write_fb_cell(unsigned short int x, unsigned short int y, char character, unsigned char bg, unsigned char fg);

/*Move the framebuffer cursor to the specified position (x, y)
 *Implemented in kstdlib_s.s
 *Returns: 0 if successful, 1 if x out of bounds, 2 if y is out of bounds
 */
extern int set_fb_cursor(unsigned short int x, unsigned short int y);

/*Get the framebuffer curor position ( = y * FB_COLS + x)
 *Implemented in kstdlib_s.s
 */
extern int get_fb_cursor(void);

/*Write c to the framebuffer at the current cursor position, and advance the cursor
 *Implemented in kstdlib_c.c
 */
void putch(char c);

/*Write string to framebuffer at current cursor position. Cursor is advanced to cell after last character
 *Implemented in kstdlib_c.c;
 */
void puts(char* c);

#endif
