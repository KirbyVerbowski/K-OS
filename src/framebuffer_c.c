#include "../header/framebuffer.h"
#include "../header/io.h"

int set_fb_cursor(unsigned char x, unsigned char y)
{
	if(x >= FB_COLS)
		return 1;
	if(y >= FB_ROWS)
		return 2;

	int curs_pos = (y * FB_COLS) + x;

	outb(FB_DESC_PORT, FB_CURS_POS_H);
	outb(FB_DATA_PORT, (unsigned char)((curs_pos & 0xFF00) >> 8));
	outb(FB_DESC_PORT, FB_CURS_POS_L);
	outb(FB_DATA_PORT, (unsigned char)(curs_pos & 0xFF));

	return 0;
}

unsigned short get_fb_cursor(void)
{
	outb(FB_DESC_PORT, FB_CURS_POS_H);
	unsigned short res = inb(FB_DATA_PORT);
	res = res << 8;
	outb(FB_DESC_PORT, FB_CURS_POS_L);
	res |= inb(FB_DATA_PORT);
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
		if(cur == '\n')
		{
			short curs = get_fb_cursor();
			char x = (char)(curs % FB_ROWS);
			char y = (char)((curs - x) / FB_ROWS);
			set_fb_cursor(0, (y+1) % FB_ROWS);
		}
		else
		{
			putch(cur);
		}

	}
}