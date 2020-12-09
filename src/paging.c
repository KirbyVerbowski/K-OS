#include "../header/paging.h"

extern void pfa_bitmap(void);

void clear_pfa_bitmap(unsigned int kernel_physical_start, unsigned int kernel_pages)
{
	unsigned int frame_number = kernel_physical_start >> 12;

	//This makes the assumption that kernel_pages is a multiple of 32 = (8 bits/page * 4 bytes set at a time)
	int * bitmap = (int *)&pfa_bitmap;

	//Clear bits before kernel (divide by 4 for ints)
	for(unsigned int i = 0; i < frame_number >> 2; i++)
	{
		bitmap[i] = 0;
	}
	//Set kernel bits (dividing kernel_pages by 32 here)
	for(unsigned int i = 0; i < kernel_pages >> 5; i++)
	{
		bitmap[i + (frame_number >> 2)] = 0xFFFFFFFF;
	}
	//Clear the rest (divide PFA_BITMAP_BYTES by 4 because we are using ints)
	for(unsigned int i = ((frame_number >> 2) + (kernel_pages >> 5)); i < PFA_BITMAP_BYTES >> 2; i++)
	{
		bitmap[i] = 0;
	}
}

//Paddr is an address in the page frame that will be freed
void free_frame(unsigned int paddr)
{
	int frame_number = paddr >> 12;
	char * bitmap = (char *)&pfa_bitmap;

	bitmap[frame_number / 8] &= ~(1 << (frame_number % 8));
}

unsigned int allocate_frame()
{
	unsigned char * bitmap = (unsigned char *)&pfa_bitmap;

	//TODO: Search 4 bytes at a time instead of 1. Be careful of endian-ness!
	for(unsigned int i = 0; i < PFA_BITMAP_BYTES; i++)
	{
		//Found a free frame
		if(bitmap[i] != 0xFF)
		{
			//Find the lowest bit that is unset
			char mask = 1;
			for(unsigned int j = 0; j < 8; j++)
			{
				if(!(bitmap[i] & mask))
				{
					bitmap[i] |= mask;
					return ((i * 8 + j) << 12);
				}
				mask = mask << 1;
			}
		}
	}

	//If we can't find a free frame, generate an interrupt
	//TODO: handle this interrupt and swap a page frame to disk
	asm("int 255");
	return 0;
}
