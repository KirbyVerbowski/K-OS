#include "../header/framebuffer.h"
#include "../header/string.h"

int kmain()
{
	/* Fill the screen with ' ' on a green background */
	for(int x = 0; x < 80; x++)
	{
		for(int y = 0; y < 25; y++)
		{
			if(y % 2){
				if(x % 2)
					write_fb_cell(x, y, ' ', FB_LIGHTMAGENTA, FB_BLACK);
				else
					write_fb_cell(x, y, ' ', FB_BLACK, FB_WHITE);
			}
			else
			{
				if(!(x % 2))
					write_fb_cell(x, y, ' ', FB_LIGHTMAGENTA, FB_BLACK);
				else
					write_fb_cell(x, y, ' ', FB_BLACK, FB_WHITE);
			}

		}
	}
	

	set_fb_cursor(0, 0);

	char* text = "text";
	char stext[] = "text";
	//int a = 0;
	//int b = 0;
	char str[12];
	//char bin[36];
	char longstr[21] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '!', '!', '1', '1', '!', '!', '1', '!', '\0'};
	
	to_string(str, (int)text, FORMAT_HEX_UPRCASE_PAD);
	puts(str);
	set_fb_cursor(0, 1);
	to_string(str, (int)stext, FORMAT_HEX_UPRCASE_PAD);
	puts(str);
	
	set_fb_cursor(0, 5);
	puts(longstr);

	return get_fb_cursor();	//1999 = 0x07CF
}
