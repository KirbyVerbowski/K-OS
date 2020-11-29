#include "../header/stdio.h"
#include "../header/keyboard.h"
#include "../header/kernel.h"

char getch()
{
	char ASCII = 0;
	volatile KeyEvent * keyEvent = (KeyEvent *)KEYBUF;
	//Wait until a key with a valid ASCII code has been pressed
	while(!((ASCII = keyEvent->ASCII) != 0 && (keyEvent->modifiers & BUFMASK_PRESSED)));

	//Consume the keypress
	keyEvent->ASCII = 0;
	keyEvent->key = None;
	keyEvent->modifiers &= ~BUFMASK_PRESSED;
	return ASCII;
}
