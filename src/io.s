global set_io					; void set_io(unsigned short int port, unsigned char value);

global get_io					; unsigned char get_io(unsigned short int port);

section .text
    set_io:
		mov  eax, [esp + 8]
		mov  edx, [esp + 4]
		out  dx, al
		ret

	get_io:
		mov  edx, [esp + 4]
		mov  eax, 0
		in   al, dx
		ret