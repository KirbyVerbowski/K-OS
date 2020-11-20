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


extern int write_fb_cell(unsigned short int x, unsigned short int y, char character, unsigned char bg, unsigned char fg);
#endif
