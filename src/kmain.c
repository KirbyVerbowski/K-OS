#include "../header/kstdlib.h"

int main(void)
{
	/* Fill the screen with 'K' on a green background */
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
	
	set_fb_cursor(1, 1);
	putch('a');
	putch('b');
	putch('c');
	putch('d');
	putch(' ');
	
	char str[13] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0'};
	
	puts(str);
	return get_fb_cursor();
}
