#include "../header/kstdio.h"

int main(void)
{
	/* Fill the screen with ' ' on a green background */
	for(int x = 0; x < 80; x++)
	{
		for(int y = 0; y < 25; y++)
		{
			if(write_fb_cell(x, y, ' ', FB_GREEN, FB_BLACK))
			{
				return -1;
			}
		}
	}
	
	/*
	putch('a');
	putch('b');
	putch('c');
	putch('d');
	putch(' ');
	
	char str[13] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0'};
	
	puts(str);

	char hexFormat[3] = {'0', 'X', 0};
	for(int i = 0; i < 16; i++)
	{
		putint(i, hexFormat);
		putch(' ');
	}
	*/

	//char* str2 = "xyz";
	//puts(str2);
	/*for(int i = 0; i < 4; i++)
	{
		putch
	}*/
	set_fb_cursor(79, 24);
	return get_fb_cursor();	//1999 = 0x07CF
}
