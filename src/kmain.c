#include "../header/framebuffer.h"
#include "../header/string.h"
#include "../header/io.h"
#include "../header/interrupt.h"
#include "../header/keyboard.h"
#include "../header/kernel.h"

void setup_gdt(char * gdt_location);

int kmain(char * idt_location, char * gdt_location)
{	
	// Need to rewrite getch()	
	// KeyEvent * keyEvent = (KeyEvent *)KEYBUF;
	// keyEvent->ASCII = 0;
	// keyEvent->keyCode = 0;
	// keyEvent->modifiers = 0;
	// unsigned char * mod = (unsigned char *)KEYMOD;
	// *mod = 0;

	/* Fill the screen with pink checkerboard pattern */
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
	char ok[] = "OK";
	char err[] = "ERROR";
	set_fb_cursor(0, 0);
	char welcomeMsg[] = "K-OS boot: ";
	puts(welcomeMsg);

	set_fb_cursor(0, 1);
	char gdtmsg[] = "Setting GDT... ";
	puts(gdtmsg);
	setup_gdt(gdt_location);
	puts(ok);

	set_fb_cursor(0, 2);
	char idtmsg[] = "Setting IDT... ";
	puts(idtmsg);
	/* Set up the interrupt hanler */
	generate_idt((unsigned int *)idt_location);
	struct gdt idt_header = {
		.size = 2048,
		.address = ((unsigned int)idt_location)
	};
	load_idt(&idt_header);
	puts(ok);


	set_fb_cursor(0, 3);
	char picmsg[] = "Configuring PIC... ";
	puts(picmsg);
	/* Set up the PIC and related hardware */
	//Point the PIC interrupts at our interrupt handler
	PIC_remap(PIC1_VECTOR_OFFSET, PIC2_VECTOR_OFFSET);
	puts(ok);

	set_fb_cursor(0, 4);
	char pickbdmsg[] = "Setting PIC to handle keyboard only... ";
	puts(pickbdmsg);
	//Handle only keyboard interrupts
	IRQ_set_mask(0xFD, 0xFF);
	puts(ok);

	set_fb_cursor(0, 5);
	char kbdmsg[] = "Configuring keyboard... ";
	puts(kbdmsg);
	if(configure_keyboard())
		puts(err);
	else
		puts(ok);


	//Enable timer interrupts
	//timer_config(0x00FF);
	//IRQ_clear_mask_bit(0);	

	/* The PIC and hardware have been configured, now use them */
	set_fb_cursor(0, 6);
	char intmsg[] = "Enabling hardware interrupts... ";
	puts(intmsg);
	asm("sti");
	puts(ok);

	set_fb_cursor(0, 7);
	char completemsg[] = "Setup complete.";
	puts(completemsg);
	set_fb_cursor(0, 8);


	//Define a pointer to an address that isn't present
	int * x = (int *)0x00000045;
	//This will cause a page fault
	*x = 3;

	return 0;
}

/* Currently unused  */

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

void setup_gdt(char * gdt_location)
{
	struct segment_descriptor datadesc = {
		.base_l = 0x0000,	.base_m = 0x00,	.base_h = 0,
		.limit_l = 0xFFFF,
		.flags = 0x93,
		.limit_h_flags = 0xCF
	};

	struct segment_descriptor codedesc = {
		.base_l = 0,	.base_m = 0,	.base_h = 0,
		.limit_l = 0xFFFF,
		.flags = 0x9B,
		.limit_h_flags = 0xCF
	};

	struct segment_descriptor nulldesc = {
		.base_l = 0,	.base_m = 0,	.base_h = 0,
		.limit_l = 0,
		.flags = 0,
		.limit_h_flags = 0
	};

	set_gdt_info((struct gdt *)gdt_location, 3);
	set_gdt_entry(gdt_location, 0x0000, nulldesc);
	set_gdt_entry(gdt_location, 0x0008, codedesc);
	set_gdt_entry(gdt_location, 0x0010, datadesc);
	
	load_gdt((struct gdt *)gdt_location);
}