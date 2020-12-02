global loader
extern kmain		; kmain.c : int kmain() 

MAGIC_NUMBER		equ 0x1BADB002          ; GRUB looks for this number
FLAGS   	        equ 0x0     			; No flags
CHECKSUM			equ -MAGIC_NUMBER
KERNEL_STACK_SIZE	equ 4096				; Bytes allocated for C stack
GDT_MAXSIZE			equ 1024				; Bytes allocated for the GDT
IDT_SIZE			equ 2048				; Bytes allocated for the IDT
PAGE_DIR_SIZE		equ 4096				; Bytes allocated for the Page directory table
PAGE_TBL_SIZE		equ 4096				; Bytes allocated for a page table

section .bss
	align 4
	idt_start:
		resb IDT_SIZE						; Reserve memory for the IDT				
	gdt_start:
		resb GDT_MAXSIZE					; Reserve memory for the GDT (unused currently)
		resb 4096 - (GDT_MAXSIZE + IDT_SIZE)	; Pad to 4KB
	page_directory:
		resb PAGE_DIR_SIZE					; Reserve memory for the PDT
	page_table_0:
		resb PAGE_TBL_SIZE					; TODO: Remove this when I make a page frame allocator
	page_table_1:							; For now, identity map the first MB used by BIOS/GRUB
		resb PAGE_TBL_SIZE					; And the second MB used by the kernel (for now)
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

		call idmap8MB						; Identity map the first two MB in page tables

		mov eax, page_directory				; Enable paging
		mov cr3, eax
		mov eax, cr0
		or  eax, 0x80000001					; Set paging and protection bits in CR0
		mov cr0, eax

		mov ebx, gdt_start
		push ebx
		mov ebx, idt_start
		push ebx
		call kmain
		pop ebx
		pop ebx

		jmp loop	


	;
	; Identity maps the first 8MB of memory into pages.
	;
	idmap8MB:
		; First directory entry
		mov eax, page_table_0
		and eax, ~0xB
		or  eax, 0x0000000B			; Set flags for this table (R/W, supervisor only, write-thru cache, 4KiB pages)
		mov [page_directory], eax
		; Second directory entry (same as above)
		mov eax, page_table_1
		and eax, 0xFFFFF000	
		or  eax, 0x0000000B	
		mov [page_directory + 4], eax

		mov ebx, page_directory + 8
		mov eax, 0x0000000A			; Same flags, but not present
	fill_directory:
		mov [ebx], eax
		add ebx, 4
		cmp ebx, page_directory + PAGE_DIR_SIZE
		jne fill_directory
		
		;Now set up the page tables that we just pointed to
		mov eax, 0x0000000B			; Same flag settings as the directory table
		mov ebx, page_table_0
	fill_table_0:
		mov [ebx], eax
		add eax, 0x1000					; Next 4K aligned address
		add ebx, 4
		cmp ebx, page_table_0 + PAGE_TBL_SIZE
		jne fill_table_0
		mov ebx, page_table_1
	fill_table_1:
		mov [ebx], eax
		add eax, 0x1000
		add ebx, 4
		cmp ebx, page_table_1 + PAGE_TBL_SIZE
		jne fill_table_1
		ret
		
	loop:                                  		; Idle forever
		    jmp loop       
