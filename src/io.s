section .text

	global outb						; void outb(unsigned short int port, unsigned char value);

	global inb						; unsigned char inb(unsigned short int port);

	global outw

	global inw

	global outd

	global ind

	global load_gdt                 ; void load_gdt(struct gdt *table);

	global getstackptr				; char * getstackptr();

	global load_idt					; void load_idt(char * tableptr);

	global get_CR2					; unsigned int get_CR2();
	get_CR2:
		mov eax, cr2
		ret

	load_idt:
		mov  eax, [esp + 4]
		lidt [eax]
		ret

	getstackptr:
        mov eax, esp
        add eax, 4      ; ret will take 4 bytes off the stack
        ret
		
    outb:
		mov  eax, [esp + 8]
		mov  edx, [esp + 4]
		out  dx, al
		ret

	inb:
		mov  edx, [esp + 4]
		mov  eax, 0
		in   al, dx
		ret

	outw:
		mov  eax, [esp + 8]
		mov  edx, [esp + 4]
		out  dx, ax
		ret

	inw:
		mov  edx, [esp + 4]
		mov  eax, 0
		in   ax, dx
		ret

	outd:
		mov  eax, [esp + 8]
		mov  edx, [esp + 4]
		out  dx, eax
		ret

	ind:
		mov  edx, [esp + 4]
		in   eax, dx
		ret

    load_gdt:
        cli 
        mov  eax, [esp + 4]
        lgdt [eax]                      ; Load the gdt passed by parameter

		mov eax, cr0
		or al, 1
		mov cr0, eax

		jmp 0x08:.flush_cs                   ; Set up code segment selector (Kernel privilege)
		.flush_cs:
			mov eax, 0x00000010

			mov ds, ax						; Set up data segment selectors (Kernel privilege)
			mov es, ax
			mov fs, ax
			mov gs, ax	
			sti
			mov ss, ax
			ret

