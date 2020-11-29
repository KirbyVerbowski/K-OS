#include "../header/heap.h"

char * heap_top = 0;

/* Get the current stack pointer address
 * Implemented in io.s
 */
extern char * getstackptr();

void heap_init(char * baseptr)
{
    heap_top = baseptr;
}

/* This has all kinds of problems. Its simply meant to be a way to get persistent pointers
 * between stack frames.
 * 
 * If the stack becomes so large that it collides with the heap, THE HEAP WILL BE OVERWRITTEN
 */
void * malloc(unsigned int bytes)
{
    char * returnAddress = heap_top;

    //Trying to allocate memory that belongs to the stack
    if(!heap_top || (unsigned int)returnAddress + bytes > (unsigned int)getstackptr())
    {
        return 0;
    }
    else
    {
        heap_top = (char *)((unsigned int)heap_top + bytes);
        return returnAddress;
    }
    
}