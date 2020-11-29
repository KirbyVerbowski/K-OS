#include "../header/keyboard.h"
#include "../header/io.h"

#define PS2_CMD_PORT    0x64
#define PS2_DATA_PORT   0x60

#define INPUT_BUFFER_FLAG       0x02
#define OUTPUT_BUFFER_FLAG      0x01
#define CMD_SET_CFG_BYTE        0x60
#define CMD_TEST_PORT_1         0xAB
#define PS2_DEVICE_ACK          0xFA
#define ENABLE_DEV_1_INT        0b01100101
#define CMD_RESET_DEVICE        0xFF
#define DEVICE_SELFTEST_PASS    0xAA
#define DEVICE_SELFTEST_FAIL1   0xFC
#define DEVICE_SELFTEST_FAIL2   0xFD
#define DEVICE_RESEND           0xFE

#define CMD_IDENTIFY_DEVICE     0xF2
#define CMD_CONFIG_TYPEMATIC    0xF3
#define CMD_ENABLE_SCANNING     0xF4

#define MIN_TYPEMATIC_INTERVAL  0x00    /* 30Hz repeat rate, 250ms delay before repeat */

char kbd_US [128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
  '\t', /* <-- Tab */
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     
    0, /* <-- control key */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0  /* All other keys are undefined */
};

unsigned char read_scancode()
{
    return inb(PS2_DATA_PORT);
}

unsigned char read_ASCII()
{
    return scancode2ASCII(read_scancode());
}

unsigned char scancode2ASCII(unsigned char scancode)
{
    if(scancode >= 128)
        return 0;
    else
        return kbd_US[scancode];
}

int configure_keyboard()
{
    unsigned char response = 0;

    /* Enable only the keyboard port for interrupts */

	//Send the command to set the configuration byte
	outb(PS2_CMD_PORT, CMD_SET_CFG_BYTE);
	//Write the configuration byte to the controller (enable first PS/2 port interrupts)
	outb(PS2_DATA_PORT, ENABLE_DEV_1_INT);

	//Test the first PS/2 port
	outb(PS2_CMD_PORT, CMD_TEST_PORT_1);
	//Busy-wait until the controller has a byte to read
	while(!(inb(PS2_CMD_PORT) & OUTPUT_BUFFER_FLAG));
	
	response = inb(PS2_DATA_PORT);
	if(response)
		return -1;	

    
    /* Reset keyboard */
    do
    {        
        //Wait for input buffer to clear
	    while((inb(PS2_CMD_PORT) & INPUT_BUFFER_FLAG));
        outb(PS2_DATA_PORT, CMD_RESET_DEVICE);
        //Wait for data to be written to the output buffer
        while(!(inb(PS2_CMD_PORT) & OUTPUT_BUFFER_FLAG));
        //Read response (should be ACK)
        response = inb(PS2_DATA_PORT);

        if(response != PS2_DEVICE_ACK)
            return -1;

        //Wait for data to be written to the output buffer
        while(!(inb(PS2_CMD_PORT) & OUTPUT_BUFFER_FLAG));
        //Read response (device status)
        response = inb(PS2_DATA_PORT);
    } while (response == DEVICE_RESEND);
    
    if(response != DEVICE_SELFTEST_PASS)
        return -1;


    /* Configure typematic rate/delay */

    //Wait for input buffer to clear
    while((inb(PS2_CMD_PORT) & INPUT_BUFFER_FLAG));
    outb(PS2_DATA_PORT, CMD_CONFIG_TYPEMATIC);
    //Wait and send configuration byte
    while((inb(PS2_CMD_PORT) & INPUT_BUFFER_FLAG));
    outb(PS2_DATA_PORT, MIN_TYPEMATIC_INTERVAL);
    //Wait for data to be written to the output buffer
    while(!(inb(PS2_CMD_PORT) & OUTPUT_BUFFER_FLAG));
    //Read response (should be ACK)
    response = inb(PS2_DATA_PORT);

    if(response != PS2_DEVICE_ACK)
            return -1;

    /*Enable scanning */

    //Wait for input buffer to clear
    while((inb(PS2_CMD_PORT) & INPUT_BUFFER_FLAG));
    outb(PS2_DATA_PORT, CMD_ENABLE_SCANNING);
    //Wait for data to be written to the output buffer
    while(!(inb(PS2_CMD_PORT) & OUTPUT_BUFFER_FLAG));
    //Read response (should be ACK)
    response = inb(PS2_DATA_PORT);

    if(response != PS2_DEVICE_ACK)
            return -1;



	return 0;
}