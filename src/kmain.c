#include "../header/framebuffer.h"
#include "../header/string.h"
#include "../header/io.h"

void set_gdt_entry(char *table_start, unsigned short byteIndex, struct segment_descriptor segment)
{
	//Get a pointer to the area in memory where this descriptor will go (Table is 6 bytes long)
	struct segment_descriptor *memseg = (struct segment_descriptor *)(table_start + 6 + byteIndex);

	//Copy all values out to memory
	memseg->base_l = segment.base_l;
	memseg->base_m = segment.base_m;
	memseg->base_h = segment.base_h;
	memseg->limit_l = segment.limit_l;
	memseg->limit_h_flags = segment.limit_h_flags;
	memseg->flags = segment.flags;
}

void set_gdt_info(struct gdt *table_start, unsigned short no_entries)
{
	//Address of first table entry (Table itself is 6 bytes long)
	table_start->address = (unsigned int)((char *)table_start + 6);
	//Size is in bytes
	table_start->size = 8 * no_entries;
}

int kmain(char *table_location)
{	
	struct segment_descriptor datadesc = {
		.base_l = 0x0000,	.base_m = 0x00,	.base_h = 0,
		.limit_l = 0x0103,
		.flags = 0x93,
		.limit_h_flags = 0xE0
	};

	struct segment_descriptor codedesc = {
		.base_l = 0,	.base_m = 0,	.base_h = 0,
		.limit_l = 0x0103,
		.flags = 0x9B,
		.limit_h_flags = 0xE0
	};

	struct segment_descriptor nulldesc = {
		.base_l = 0,	.base_m = 0,	.base_h = 0,
		.limit_l = 0,
		.flags = 0,
		.limit_h_flags = 0
	};

	set_gdt_info((struct gdt *)table_location, 3);
	set_gdt_entry(table_location, 0x0000, nulldesc);
	set_gdt_entry(table_location, 0x0008, codedesc);
	set_gdt_entry(table_location, 0x0010, datadesc);
	
	load_gdt((struct gdt *)table_location);

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

	//int myint = 11;
	char* text = "text";
	char stext[] = "text";
	//int a = 0;
	//int b = 0;
	char str[12];
	//char bin[36];
	char longstr[21] = "Hello world !!!11!!1";
	
	to_string(str, (int)text, FORMAT_HEX_UPRCASE_PAD);
	puts(str);
	set_fb_cursor(0, 1);
	to_string(str, (int)stext, FORMAT_HEX_UPRCASE_PAD);
	puts(str);
	
	set_fb_cursor(0, 5);
	puts(longstr);

	set_fb_cursor(0, 7);
	//This causes a 'segfault' because its stored in a very high memory address
	//putch(*text);

	return 0;
}
