#include "../header/paging.h"

//Pointers to memory locations reserved during linking
extern void kernel_page_table(void);
extern void page_directory(void);
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

//Maps the page frame which contains paddr to the page containing vaddr
void map_page(unsigned int vaddr, unsigned int paddr, int user_page, int readwrite)
{
	unsigned int * pdir = (unsigned int *)&page_directory;	

	//There is already a page directory entry present
	if(pdir[vaddr >> 22] & PDE_PRESENT)
	{
		//Temporarily map the allocator frame to the page table we need to update
		unsigned int * kernel_ptable = (unsigned int *)&kernel_page_table;
		kernel_ptable[1023] = (pdir[vaddr >> 22] & PTE_ADDR) | PTE_PRESENT | PTE_CACHE_WRITETHRU | PTE_READWRITE;

		//This is now a pointer to the table we need
		unsigned int * ptable = (unsigned int *)ALLOCATOR_PAGE;

		//There is already a page table entry present
		if(ptable[(vaddr >> 12) & 0x000003FF] & PTE_PRESENT)
		{
			//If this virtual address is already in use, generate an interrupt
			//TODO: keep track of used pages
			asm("int 254");
		}

		//Set the entry to point at the physical address
		ptable[(vaddr >> 12) & 0x000003FF] = (paddr & PTE_ADDR) | PTE_PRESENT | PTE_CACHE_WRITETHRU;
		if(readwrite)
			ptable[(vaddr >> 12) & 0x000003FF] |= PTE_READWRITE;
		if(user_page)
			ptable[(vaddr >> 12) & 0x000003FF] |= PTE_USERACCESS;

		//Unmap the allocator frame
		kernel_ptable[1023] = PTE_CACHE_WRITETHRU | PTE_READWRITE;
	}
	else
	{
		//Need to allocate a frame to hold the new page table
		unsigned int new_ptable_physaddr = allocate_frame();

		//We will temporarily map the allocator frame to get access to it
		unsigned int * kernel_ptable = (unsigned int *)&kernel_page_table;
		kernel_ptable[1023] = (new_ptable_physaddr & PTE_ADDR) | PTE_PRESENT | PTE_CACHE_WRITETHRU | PTE_READWRITE;

		//This is now a pointer to the frame holding the new page table
		unsigned int * new_ptable = (unsigned int *)ALLOCATOR_PAGE;

		//Set all entries empty and non-present
		for(int i = 0 ; i < 1024; i++)
		{
			new_ptable[i] = PTE_CACHE_WRITETHRU;
			if(readwrite)
				new_ptable[i] |= PTE_READWRITE;
			if(user_page)
				new_ptable[i] |= PTE_USERACCESS;
		}
		//Add the real entry
		new_ptable[(vaddr >> 12) & 0x000003FF] = (paddr & PTE_ADDR) | PTE_PRESENT | PTE_CACHE_WRITETHRU;
		if(readwrite)
			new_ptable[(vaddr >> 12) & 0x000003FF] |= PTE_READWRITE;
		if(user_page)
			new_ptable[(vaddr >> 12) & 0x000003FF] |= PTE_USERACCESS;

		//Make a page directory entry to point to the table
		pdir[vaddr >> 22] = (new_ptable_physaddr & PTE_ADDR) | PDE_PRESENT | PDE_CACHE_WRITETHRU;
		if(readwrite)
			pdir[vaddr >> 22] |= PDE_READWRITE;
		if(user_page)
			pdir[vaddr >> 22] |= PDE_USERACCESS;

		//Unmap the allocator frame
		kernel_ptable[1023] = PTE_CACHE_WRITETHRU | PTE_READWRITE;
	}
	
}