global set_io					; void set_io(unsigned short int port, unsigned char value);

global get_io					; unsigned char get_io(unsigned short int port);

global load_gdt                 ; void load_gdt(struct gdt *table);

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

    load_gdt:
        cli 
        mov  eax, [esp + 4]
        lgdt [eax]                      ; Load the gdt passed by parameter
		
		mov eax, cr0
		or al, 1
		mov cr0, eax

        mov eax, 0x00000010

		mov ds, ax						; Set up data segment selectors (Kernel privilege)
		mov ss, ax


		mov es, ax
		mov fs, ax
		mov gs, ax

		jmp 0x08:flush_cs                   ; Set up code segment selector (Kernel privilege)
	flush_cs:
        sti
        ret