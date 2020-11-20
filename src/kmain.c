#include "kstdlib.h"

int main(void)
{
	/* Fill the screen with 'K' on a green background */
	for(int x = 0; x < 80; x++)
	{
		for(int y = 0; y < 25; y++)
		{
			if(write_fb_cell(x, y, 'K', 2, 0))
			{
				return -1;
			}
		}
	}
	
	move_fb_cursor(5, 0);
	return 0;
}
