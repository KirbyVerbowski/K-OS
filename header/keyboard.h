#ifndef HEADER_keyboard
#define HEADER_keyboard

#define BUFMASK_PRESSED 0x80
#define BUFMASK_CONSUMED 0x40
#define BUFMASK_LCTRL 0x20
#define BUFMASK_RCTRL 0x10
#define BUFMASK_LSHFT 0x08
#define BUFMASK_RSHFT 0x04
#define BUFMASK_LALT 0x02
#define BUFMASK_RALT 0x01
#define BUFMASK_CTRL (BUFMASK_LCTRL | BUFMASK_RCTRL)
#define BUFMASK_SHFT (BUFMASK_LSHFT | BUFMASK_RSHFT)
#define BUFMASK_ALT (BUFMASK_LALT | BUFMASK_RALT)

/** BUFMASK:
  * Bit  |   7    |   6    |  5  |  4  |  3 |  2 |  1 |  0
  *       Pressed? Consumed LCTRL RCTRL LSFT RSFT LALT RALT
  */


typedef enum KeyCode
{
    None, Esc, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, PrtScn, ScrLk, Pause,
    BackTick, D1, D2, D3, D4, D5, D6, D7, D8, D9, D0, Minus, Equals, Backspace, Insert, Home, PgUp, NumLock, KeypadSlash, KeypadAsterisk, KeypadMinus,
    Tab, Q, W, E, R, T, Y, U, I, O, P, LeftBracket, RightBracket, Backslash, Delete, End, PgDown, Keypad7, Keypad8, Keypad9, KeypadPlus,
    CapsLock, A, S, D, F, G, H, J, K, L, Semicolon, Quote, Enter, Keypad4, Keypad5, Keypad6,
    LeftShift, Z, X, C, V, B, N, M, Comma, Period, Forwardslash, RightShift, Up, Keypad1, Keypad2, Keypad3, KeypadEnter,
    LeftCtrl, LeftWindows, LeftAlt, Space, RightAlt, RightWindows, Menu, RightCtrl, Left, Down, Right, Keypad0, KeypadPeriod
} KeyCode;

//6 bytes
typedef struct KeyEvent
{
	unsigned int keyCode;
	unsigned char ASCII;
	unsigned char modifiers;
}__attribute__((packed)) KeyEvent;


/** Parses scancodes into KeyEvents. Called when a keyboard interrupt is generated
 *  Implemented in keyboard.c
 */
void handle_keypress();

/** Simple hardware check and reset of the PS/2 Keyboard, enables scancode set 1
 *  Implemented in keyboard.c
 */
int configure_keyboard();

/** Blocks until an ASCII key is pressed
 *  Implemented in keyboard.c
 */
char getch();

#endif