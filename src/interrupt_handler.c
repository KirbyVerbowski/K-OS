#include "../header/interrupt.h"
#include "../header/io.h"
#include "../header/framebuffer.h"
#include "../header/keyboard.h"

#define ICW1_ICW4	    0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	    0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL  	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	    0x10		/* Initialization - required! */
 
#define ICW4_8086   	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	    0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM   	0x10		/* Special fully nested (not) */

#define PIT_CHANNEL0        0x40
#define PIT_COMMAND_REG     0x43

#define PIT_CMD_CH0         0x00
#define PIT_CMD_CH1         0x40
#define PIT_CMD_CH2         0x80

#define PIT_CMD_ACCESSLBHB  0x30

#define PIT_CMD_OPMODE_INT  0x00

#define PIT_CMD_MODE_BIN    0x00
#define PIT_CMD_MODE_BCD    0x01

void interrupt_handler(unsigned int interrupt, unsigned int errorcode)
{
    //Write out the values for debug
    unsigned int * sti = (unsigned int *)0x00200000;
    unsigned int * ste = (unsigned int *)0x00200004;
    *ste = errorcode;
    *sti = interrupt;

    //PIC interrupt
    if(interrupt >= PIC1_VECTOR_OFFSET && interrupt < PIC2_VECTOR_OFFSET + 8)
    {      
        //Keyboard interrupt
        if(interrupt == PIC1_VECTOR_OFFSET+1){
            PIC_sendEOI(interrupt - PIC1_VECTOR_OFFSET);
            //print the typed key if it has an ASCII encoding
            char ascii = read_ASCII();       
            if(ascii)     
                putch(ascii);
        }

    }
    else{
        //Interrupt came from an unknown source
        DEBUG_BREAKPOINT();
    }

}

void timer_config(unsigned short reload)
{
    asm("cli");
    //Setup channel 0 to trigger IRQ0 on reload
    outb(PIT_COMMAND_REG, PIT_CMD_MODE_BIN | PIT_CMD_OPMODE_INT | PIT_CMD_ACCESSLBHB | PIT_CMD_CH0);
    //Assign the reload value
    outb(PIT_CHANNEL0, reload & 0x00FF);
    outb(PIT_CHANNEL0, ((reload & 0xFF00) >> 8));
    asm("sti");
}

void PIC_sendEOI(unsigned char irq)
{
    if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);
 
	outb(PIC1_COMMAND,PIC_EOI);
}

//Note: this code (and below functions) will not work on older machines (no I/O waiting)
void PIC_remap(unsigned char vecOffset_PIC1, unsigned char vecOffset_PIC2)
{
    unsigned char a1, a2;
 
	a1 = inb(PIC1_DATA);                             // save masks
	a2 = inb(PIC2_DATA);
 
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);       // starts the initialization sequence (in cascade mode)	
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(PIC1_DATA, vecOffset_PIC1);                 // ICW2: Master PIC vector offset
	outb(PIC2_DATA, vecOffset_PIC2);                 // ICW2: Slave PIC vector offset
	outb(PIC1_DATA, 4);                              // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb(PIC2_DATA, 2);                              // ICW3: tell Slave PIC its cascade identity (0000 0010)
 
	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);
 
	outb(PIC1_DATA, a1);                            // restore saved masks.
	outb(PIC2_DATA, a2);
}

void IRQ_set_mask(unsigned char PIC1_mask, unsigned char PIC2_mask)
{
    outb(PIC1_DATA, PIC1_mask);
    outb(PIC2_DATA, PIC2_mask);
}

void IRQ_set_mask_bit(unsigned char IRQline)
{
    unsigned short port;
    unsigned char value;
 
    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);        
}
 
void IRQ_clear_mask_bit(unsigned char IRQline) 
{
    unsigned short  port;
    unsigned char value;
 
    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);        
}