#ifndef HEADER_io
#define HEADER_io

#define DEBUG_BREAKPOINT() asm("xchg bx, bx")

//Global descriptor table
struct gdt {
    unsigned short size;
    unsigned int address;
} __attribute__((packed));

//Segment descriptor
struct segment_descriptor{
    unsigned short limit_l;
    unsigned short base_l;
    unsigned char base_m;
    unsigned char flags;
    unsigned char limit_h_flags;
    unsigned char base_h;
} __attribute__((packed));

/*Write value to I/O port
 *Implemented in io.s
 */
extern void set_io(unsigned short port, unsigned char value);

/*Read value from I/O port
 *Implemented in io.s
 */
extern unsigned char get_io(unsigned short port);

/*Load the global descriptor table in to the CPU
 *Implemented in io.s
 */
extern void load_gdt(struct gdt *table);
#endif