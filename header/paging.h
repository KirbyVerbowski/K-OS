#ifndef HEADER_paging
#define HEADER_paging

#include "kernel.h"

#define PDE_ADDR			0xFFFFF000
#define PDE_AVAILABLE		0x00000E00
#define PDE_IGNORED 		0x00000100
#define PDE_4MBPAGES		0x00000080
#define PDE_ZERO			0x00000040
#define PDE_ACCESSED		0x00000020
#define PDE_CACHE_DISABLE	0x00000010
#define PDE_CACHE_WRITETHRU	0x00000008
#define PDE_USERACCESS		0x00000004
#define PDE_READWRITE		0x00000002
#define PDE_PRESENT			0x00000001

#define PTE_ADDR			0xFFFFF000
#define PTE_AVAILABLE		0x00000E00
#define PTE_GLOBAL	 		0x00000100
#define PTE_ZERO			0x00000080
#define PTE_DIRTY			0x00000040
#define PTE_ACCESSED		0x00000020
#define PTE_CACHE_DISABLE	0x00000010
#define PTE_CACHE_WRITETHRU	0x00000008
#define PTE_USERACCESS		0x00000004
#define PTE_READWRITE		0x00000002
#define PTE_PRESENT			0x00000001

//Last page of the kernel is reserved for allocation purposes
#define ALLOCATOR_PAGE		(KERNEL_OFFSET + KERNEL_SIZE - 0x1000)

#define PFA_BITMAP_BYTES 0x20000

/** Mark all page frames free except for those which the kernel is occupying.
 * 	This function makes the assumption that kernel_pages is a multiple of 32
 */
void clear_pfa_bitmap(unsigned int kernel_physical_start, unsigned int kernel_pages);

/** Return a page frame to the allocator
 *  paddr is a physical address within the frame being freed
 */
void free_frame(unsigned int paddr);

/** Allocates a frame and returns its physical address
 */
unsigned int allocate_frame();

/** Creates a virtual address mapping from vaddr to paddr
 *  If the virtual address is already mapped, it will be overwritten. (This needs to be changed when user-level programs are added)
 */
void map_page(unsigned int vaddr, unsigned int paddr, int user_page, int readwrite);

#endif