#ifndef HEADER_io
#define HEADER_io

//Bochs will pause execution when it encounters this instruction
#define DEBUG_BREAKPOINT() asm("xchg bx, bx")

#define GDT_MAXSIZE 1024

//Global descriptor table
struct gdt {
    unsigned short size;
    unsigned int address;
} __attribute__((packed));

struct segment_descriptor{
    unsigned short limit_l;
    unsigned short base_l;
    unsigned char base_m;
    unsigned char flags;
    unsigned char limit_h_flags;
    unsigned char base_h;
} __attribute__((packed));

/*Write byte to I/O port
 *Implemented in io.s
 */
extern void outb(unsigned short port, unsigned char value);

/*Read byte from I/O port
 *Implemented in io.s
 */
extern unsigned char inb(unsigned short port);

/*Write word to I/O port
 *Implemented in io.s
 */
extern void outw(unsigned short port, unsigned short value);

/*Read word from I/O port
 *Implemented in io.s
 */
extern unsigned short inw(unsigned short port);

/*Write double word to I/O port
 *Implemented in io.s
 */
extern void outd(unsigned short port, unsigned int value);

/*Read double word from I/O port
 *Implemented in io.s
 */
extern unsigned int ind(unsigned short port);

/*Returns the value held in Command Register 2 (page fault register)
 *Implemented in io.s
 */
extern unsigned int get_CR2();

/*Load the global descriptor table in to the CPU
 *Implemented in io.s
 */
extern void load_gdt(struct gdt *table);

/*Load the interrupt descriptor table in to the CPU
 *Implemented in io.s
 */
extern void load_idt(struct gdt *table);

#endif