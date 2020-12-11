#include "../header/kernel.h"
#include "../header/paging.h"

//For debugging
#include "../header/string.h"
#include "../header/framebuffer.h"

#define NULL 0

typedef struct HeapNode
{
	struct HeapNode * prev;
	struct HeapNode * next;
	unsigned int size;
}__attribute__((packed)) HeapNode;


extern HeapNode * heap_head;
extern HeapNode * heap_tail;

//Implementation using a linkedlist which is naturally sorted by their pointers
//Memory looks like [node]( --- Reserved bytes --- )(potential empty space from free())[node]( --- Reserved bytes --- )...
//Because of the implementation, this always reserves bytes + sizeof(HeapNode)
void * malloc(unsigned int bytes)
{

	//Do some initialization here. Allocate a page frame for the heap to reside in, make a node and reserve space
	if(heap_head == NULL)
	{
		//Allocate the frame
		unsigned int paddr = allocate_frame();
		

		//TODO: change this
		//Mapping above the kernel for now
		map_page(paddr + KERNEL_OFFSET, paddr, 0, 1);	//This causes a page fault		

		//Point the head/tail node to the first address in the page
		heap_head = (HeapNode *)(paddr + KERNEL_OFFSET);
		heap_tail = heap_head;
		heap_head->prev = NULL;
		heap_head->next = NULL;
		heap_head->size = bytes + sizeof(HeapNode);

		return (void *)((unsigned int)heap_head + sizeof(HeapNode));
	}
	else
	{
		//When the first element in the list is freed, its size is set to 0
		if(heap_head->size == 0)
		{
			//The list is empty
			if(heap_head->next == NULL)
			{
				heap_head->size = bytes + sizeof(HeapNode);
				return (void *)((unsigned int)heap_head + sizeof(HeapNode));
			}

			//Is there enough room before the next node
			if(((unsigned int)(heap_head) + bytes + sizeof(HeapNode)) <= (unsigned int)heap_head->next)
			{
				heap_head->size = bytes + sizeof(HeapNode);
				return (void *)((unsigned int)heap_head + sizeof(HeapNode));
			}
		}


		HeapNode * node = heap_head;

		while(node != NULL)
		{
			//We are at the end of the list, insert here
			if(node->next == NULL)
			{
				//If the next allocation is not going to fit in this frame
				if(((unsigned int)node & 0x0000F000) != (((unsigned int)node + node->size + bytes + sizeof(HeapNode)) & 0x0000F000))
				{
					//Allocate a new page frame
					unsigned int paddr = allocate_frame();
					//Map it contiguously
					map_page(((unsigned int)node + 0x1000) & 0xFFFFF000, paddr, 0, 1);

					//The next node goes at the start of the next page to avoid writing over a page boundary
					node->next = (HeapNode *)(((unsigned int)node + 0x1000) & 0xFFFFF000);
				}
				else
				{
					node->next = (HeapNode *)((unsigned int)node + node->size);
				}

				heap_tail = node->next;
				heap_tail->size = bytes + sizeof(HeapNode);
				heap_tail->prev = node;
				heap_tail->next = NULL;

				return (void *)((unsigned int)heap_tail + sizeof(HeapNode));				
			}
			else
			{
				//Is there enough room between this reserved block and the next
				if((unsigned int)node->next - ((unsigned int)(node) + node->size) >= bytes + sizeof(HeapNode))
				{
					//Make sure we aren't crossing a page boundary
					if(((unsigned int)node & 0x0000F000) == (((unsigned int)node + node->size + bytes + sizeof(HeapNode)) & 0x0000F000)) 
					{
						HeapNode * newNode = (HeapNode *)((unsigned int)node + node->size);
						newNode->prev = node;
						newNode->next = node->next;
						newNode->size = bytes + sizeof(HeapNode);

						node->next->prev = newNode;
						node->next = newNode;

						return (void *)((unsigned int)newNode + sizeof(HeapNode));
					}
				}
			}
			node = node->next;
		}
	}	

	//This should never happen
	return NULL;
}


void free(void * ptr)
{
	HeapNode * node = heap_head;

	while(node != NULL)
	{
		if((void *)((unsigned int)node + sizeof(HeapNode)) == ptr)
		{
			if(node == heap_head)
			{
				node->size = 0;
				return;
			}
			else
			{
				if(node == heap_tail)
				{
					heap_tail = node->prev;
					heap_tail->next = NULL;
					return;
				}
				else
				{
					node->prev->next = node->next;
					node->next->prev = node->prev;
					return;
				}				
			}			
		}
		node = node->next;
	}
}