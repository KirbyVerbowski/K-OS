#include "../header/kstdio.h"

void putch(char c)
{
	int cursor = get_fb_cursor();
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
