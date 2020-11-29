global loader
extern kmain		; kmain.c : int kmain() 

MAGIC_NUMBER		equ 0x1BADB002          ; GRUB looks for this number
FLAGS   	        equ 0x0     			; No flags
CHECKSUM			equ -MAGIC_NUMBER
KERNEL_STACK_SIZE	equ 4096				; Bytes allocated for C stack
GDT_MAXSIZE			equ 1024				; Bytes allocated for the GDT
IDT_SIZE			equ 2048				; Bytes allocated for the IDT

section .bss
	align 4
	idt_start:
		resb IDT_SIZE						; Reserve memory for the IDT
	gdt_start:
		resb GDT_MAXSIZE					; Reserve memory for the GDT (unused currently)
	kernel_stack:
		resb KERNEL_STACK_SIZE				; Reserve memory for stack

section .text                
	align 4
    dd MAGIC_NUMBER  		               	; Write values
    dd FLAGS
    dd CHECKSUM
	
	loader:
		mov esp, kernel_stack + KERNEL_STACK_SIZE	; Initialize stack pointers
		mov ebp, esp

		mov ebx, gdt_start
		push ebx
		mov ebx, idt_start
		push ebx
		call kmain
		pop ebx
		pop ebx

		jmp loop	
		
	loop:                                  		; Idle forever
		    jmp loop       
