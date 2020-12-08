#ifndef HEADER_pageframeallocator
#define HEADER_pageframeallocator

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

#endif