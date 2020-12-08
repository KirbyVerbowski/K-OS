#include "../header/keyboard.h"
#include "../header/io.h"

#define PS2_CMD_PORT 0x64
#define PS2_DATA_PORT 0x60

#define INPUT_BUFFER_FLAG 0x02
#define OUTPUT_BUFFER_FLAG 0x01
#define CMD_SET_CFG_BYTE 0x60
#define CMD_TEST_PORT_1 0xAB
#define PS2_DEVICE_ACK 0xFA
#define ENABLE_DEV_1_INT 0b01100101
#define CMD_RESET_DEVICE 0xFF
#define DEVICE_SELFTEST_PASS 0xAA
#define DEVICE_SELFTEST_FAIL1 0xFC
#define DEVICE_SELFTEST_FAIL2 0xFD
#define DEVICE_RESEND 0xFE

#define CMD_IDENTIFY_DEVICE 0xF2
#define CMD_CONFIG_TYPEMATIC 0xF3
#define CMD_ENABLE_SCANNING 0xF4

#define MIN_TYPEMATIC_INTERVAL 0x00 /* 30Hz repeat rate, 250ms delay before repeat */



#define SCANCODE_MOREINFO 0xE0

/** Buffer of held modifier keys:
  * Bit  |   7    |6|  5  |  4  |  3 |  2 |  1 |  0
  *       Pressed? 0 LCTRL RCTRL LSFT RSFT LALT RALT
  */
//char modifierBuffer = 0x00;

//KeyEvent keyEvent = { .key = None, .ASCII = 0, .modifiers = 0};

KeyCode SingleScan2KeyCode[128] = 
{
	None, Esc, D1, D2, D3, D4, D5, D6, D7, D8, D9, D0, Minus, Equals, Backspace,Tab,
	Q, W, E, R, T, Y, U, I, O, P, LeftBracket, RightBracket, Enter, LeftCtrl, A, S,
	D, F, G, H, J, K, L, Semicolon, Quote, BackTick, LeftShift, Backslash, Z, X, C, V,
	B, N, M, Comma, Period, Forwardslash, RightShift, KeypadAsterisk, LeftAlt, Space, CapsLock, F1, F2, F3, F4, F5,
	F6, F7, F8, F9, F10, NumLock, ScrLk, Keypad7, Keypad8, Keypad9, KeypadMinus, Keypad4, Keypad5, Keypad6, KeypadPlus, Keypad1,
	Keypad2, Keypad3, Keypad0, KeypadPeriod, None, None, None, F11, F12, None, None, None, None, None, None, None,
	None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None,
	None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None
};

char SingleScan2ASCII[128] = 
{
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8, '\t',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
	'2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/** UNMAPPED KEYS:
 *  All F-keys, PrtScn, Pause, Lock keys, Insert, Home/End, PgUp/Dwn, Menu, WindowsL/R, All Multimedia buttons
 */
void handle_keypress()
{
	unsigned char scanCode = inb(PS2_DATA_PORT);
	unsigned char released = 0;
	unsigned char ASCII = 0;
	KeyCode pressedKey = None;
	unsigned char modifierBuffer = *((unsigned char *)KEYMOD);

	if (scanCode == SCANCODE_MOREINFO)
	{
		/* Handle RCTRL, RALT */
	}
	else
	{
		/* Key was released */
		if (scanCode > 0x80)
		{
			released = 1;
			modifierBuffer &= ~BUFMASK_PRESSED;
			scanCode -= 0x80;
		}
		else
		{
			released = 0;
			modifierBuffer |= BUFMASK_PRESSED;
		}
		
		pressedKey = SingleScan2KeyCode[scanCode];
		ASCII = SingleScan2ASCII[scanCode];

		switch (scanCode)
		{
			/* Handle modifier keys */

		case 0x1D:
			modifierBuffer = released ? modifierBuffer & ~BUFMASK_LCTRL : modifierBuffer | BUFMASK_LCTRL;
			break;
		case 0x2A:
			modifierBuffer = released ? modifierBuffer & ~BUFMASK_LSHFT : modifierBuffer | BUFMASK_LSHFT;
			break;
		case 0x38:
			modifierBuffer = released ? modifierBuffer & ~BUFMASK_LALT : modifierBuffer | BUFMASK_LALT;
			break;
		case 0x36:
			modifierBuffer = released ? modifierBuffer & ~BUFMASK_RSHFT : modifierBuffer | BUFMASK_RSHFT;
			break;

			/* End modifier keys */

		default:
			break;
		}
	}

	if(pressedKey != None)
	{
		//Check if we need to replace the symbol
		if(modifierBuffer & BUFMASK_SHFT)
		{
			//Alphabet character (or one of [, \, ])
			if(97 <= ASCII && ASCII <= 125)
			{
				ASCII -= 32;
			}
			else
			{
				//Hope this is all of them
				switch (ASCII)
				{
				case '0':
					ASCII = ')';
					break;
				case '1':
					ASCII = '!';
					break;
				case '2':
					ASCII = '@';
					break;
				case '3':
					ASCII = '#';
					break;
				case '4':
					ASCII = '$';
					break;
				case '5':
					ASCII = '%';
					break;
				case '6':
					ASCII = '^';
					break;
				case '7':
					ASCII = '&';
					break;
				case '8':
					ASCII = '*';
					break;
				case '9':
					ASCII = '(';
					break;
				case '`':
					ASCII = '~';
					break;
				case '-':
					ASCII = '_';
					break;
				case '=':
					ASCII = '+';
					break;
				case ';':
					ASCII = ':';
					break;
				case '\'':
					ASCII = '\"';
					break;
				case ',':
					ASCII = '<';
					break;
				case '.':
					ASCII = '>';
					break;
				case '/':
					ASCII = '?';
					break;
				
				default:
					break;
				}
			}
		}


		//Make it extended ASCII if Alt held
		if(modifierBuffer & BUFMASK_ALT)
			ASCII += 128;
		//If control is held, do not interpret as regular keypress (SIGINT coming soon)
		if(modifierBuffer & BUFMASK_CTRL)
			ASCII = 0;

	}
	
	KeyEvent * keyEvent = (KeyEvent *)KEYBUF;
	keyEvent->ASCII = ASCII;
	keyEvent->keyCode = pressedKey;
	keyEvent->modifiers = modifierBuffer;

	unsigned char * mod = (unsigned char *)KEYMOD;
	*mod = modifierBuffer;
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
	while (!(inb(PS2_CMD_PORT) & OUTPUT_BUFFER_FLAG))
		;

	response = inb(PS2_DATA_PORT);
	if (response)
		return -1;

	/* Reset keyboard */
	do
	{
		//Wait for input buffer to clear
		while ((inb(PS2_CMD_PORT) & INPUT_BUFFER_FLAG))
			;
		outb(PS2_DATA_PORT, CMD_RESET_DEVICE);
		//Wait for data to be written to the output buffer
		while (!(inb(PS2_CMD_PORT) & OUTPUT_BUFFER_FLAG))
			;
		//Read response (should be ACK)
		response = inb(PS2_DATA_PORT);

		if (response != PS2_DEVICE_ACK)
			return -1;

		//Wait for data to be written to the output buffer
		while (!(inb(PS2_CMD_PORT) & OUTPUT_BUFFER_FLAG))
			;
		//Read response (device status)
		response = inb(PS2_DATA_PORT);
	} while (response == DEVICE_RESEND);

	if (response != DEVICE_SELFTEST_PASS)
		return -1;

	/* Configure typematic rate/delay */

	//Wait for input buffer to clear
	while ((inb(PS2_CMD_PORT) & INPUT_BUFFER_FLAG))
		;
	outb(PS2_DATA_PORT, CMD_CONFIG_TYPEMATIC);
	//Wait and send configuration byte
	while ((inb(PS2_CMD_PORT) & INPUT_BUFFER_FLAG))
		;
	outb(PS2_DATA_PORT, MIN_TYPEMATIC_INTERVAL);
	//Wait for data to be written to the output buffer
	while (!(inb(PS2_CMD_PORT) & OUTPUT_BUFFER_FLAG))
		;
	//Read response (should be ACK)
	response = inb(PS2_DATA_PORT);

	if (response != PS2_DEVICE_ACK)
		return -1;

	/*Enable scanning */

	//Wait for input buffer to clear
	while ((inb(PS2_CMD_PORT) & INPUT_BUFFER_FLAG))
		;
	outb(PS2_DATA_PORT, CMD_ENABLE_SCANNING);
	//Wait for data to be written to the output buffer
	while (!(inb(PS2_CMD_PORT) & OUTPUT_BUFFER_FLAG))
		;
	//Read response (should be ACK)
	response = inb(PS2_DATA_PORT);

	if (response != PS2_DEVICE_ACK)
		return -1;


	return 0;
}