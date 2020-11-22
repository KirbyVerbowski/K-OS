global loader
extern kmain		; kmain.c : int kmain() 

MAGIC_NUMBER		equ 0x1BADB002          ; GRUB looks for this number
FLAGS   	        equ 0x0     			; No flags
CHECKSUM			equ -MAGIC_NUMBER
KERNEL_STACK_SIZE	equ 4096				; Bytes allocated for C stack

section .bss
	align 4
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

		call kmain
		jmp .loop
	
		
	.loop:                                  		; Idle forever
		    jmp .loop       
