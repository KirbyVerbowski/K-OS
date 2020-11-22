#include "../header/kstdio.h"

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
	unsigned int cursor = get_fb_cursor();
	if(cursor > 2000)
		c = '!';
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

void putint(unsigned int x, char* format)
{
	unsigned char ASCII_ZERO = 48;
	unsigned char ASCII_A = 65;
	unsigned char ASCII_a = 97;
	unsigned int mask = 0xF0000000;
	unsigned char digit = 0;
	unsigned char firstdigit_found = 0;

	//Format with leading zeros
	if(*format == '0')
	{
		switch (*(format+1))
		{
		case 'x':
			putch('0');
			putch('x');		
			
			for(int i = 0; i < 8; i++)
			{
				digit = (x & mask) >> ((8  - i)*4);
				if(digit < 10)
				{
					putch(digit + ASCII_ZERO);
				}
				else
				{
					putch(digit - 10 + ASCII_a);
				}				
				mask = mask >> 4;
			}
			break;

		case 'X':
			putch('0');
			putch('X');
			for(int i = 0; i < 8; i++)
			{
				digit = (x & mask) >> ((7 - i)*4);
				if(digit < 10)
				{
					putch(digit + ASCII_ZERO);
				}
				else
				{
					putch(digit - 10 + ASCII_A);
				}				
				mask = mask >> 4;
			}
			break;

		case 'd':
		case 'D':

			break;

		case 'b':
		case 'B':

			break;
		
		default:
			putch('?');
			break;
		}
	}
	else
	{
		switch (*format)
		{
		case 'x':
			putch('0');
			putch('x');			
			for(int i = 0; i < 8; i++)
			{
				digit = (x & mask) >> ((8 - i)*4);
				if(digit > 0){ firstdigit_found = 1; }
				if(firstdigit_found)
				{
					if(digit < 10)
					{
						putch(digit + ASCII_ZERO);
					}
					else
					{
						putch(digit - 10 + ASCII_a);
					}	
				}
							
				mask = mask >> 4;
			}
			break;
		case 'X':
			putch('0');
			putch('X');
			for(int i = 0; i < 8; i++)
			{
				digit = (x & mask) >> ((8 - i)*4);
				if(digit > 0){ firstdigit_found = 1; }
				if(firstdigit_found)
				{
					if(digit < 10)
					{
						putch(digit + ASCII_ZERO);
					}
					else
					{
						putch(digit - 10 + ASCII_A);
					}	
				}
							
				mask = mask >> 4;
			}
			break;

		case 'd':
		case 'D':

			break;

		case 'b':
		case 'B':

			break;
		
		default:
			putch('?');
			break;
		}
	}
	
}
