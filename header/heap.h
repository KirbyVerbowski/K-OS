#ifndef HEADER_heap
#define HEADER_heap


/* A *very* naive implementation of malloc. It grows like a stack. Note free() is not defined.
 * Implemented in heap.c
 */
void * malloc(unsigned int bytes);

/* Sets the base pointer for the heap. Must be called before malloc() is used.
 * Implemented in heap.c
 */
void heap_init(char * baseptr);
#endif