#include "../header/framebuffer.h"
#include "../header/io.h"

int set_fb_cursor(unsigned char x, unsigned char y)
{
	if(x >= FB_COLS)
		return 1;
	if(y >= FB_ROWS)
		return 2;

	int curs_pos = (y * FB_COLS) + x;

	set_io(FB_DESC_PORT, FB_CURS_POS_H);
	set_io(FB_DATA_PORT, (unsigned char)((curs_pos & 0xFF00) >> 8));
	set_io(FB_DESC_PORT, FB_CURS_POS_L);
	set_io(FB_DATA_PORT, (unsigned char)(curs_pos & 0xFF));

	return 0;
}

unsigned short get_fb_cursor(void)
{
	set_io(FB_DESC_PORT, FB_CURS_POS_H);
	unsigned short res = get_io(FB_DATA_PORT);
	res = res << 8;
	set_io(FB_DESC_PORT, FB_CURS_POS_L);
	res |= get_io(FB_DATA_PORT);
	return res;
}

void putch(unsigned char c)
{
	unsigned short cursor = get_fb_cursor();
	int x = cursor % FB_COLS;
	int y = (cursor - x) / FB_COLS;
	
	write_fb_cell(x, y, c, FB_BLACK, FB_WHITE);
	if(x == FB_COLS-1)
	{
		if(y == FB_ROWS-1)
			set_fb_cursor(0, 0);
		else
			set_fb_cursor(0, y+1);
	}
	else
		set_fb_cursor(x+1, y);
}

void puts(char* c)
{
	char cur;	
	while((cur = *(c++)) != '\0')
	{
		putch(cur);
	}
}