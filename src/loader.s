global loader


extern kernel_offset
extern kernel_virtual_start
extern kernel_virtual_end
extern kernel_physical_start
extern kernel_physical_end

MAGIC_NUMBER		equ 0x1BADB002          ; GRUB looks for this number
FLAGS   	        equ 0x1     			; No flags
CHECKSUM			equ -(MAGIC_NUMBER+FLAGS)
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
		resb PAGE_TBL_SIZE					; Page table for the kernel
	identity_temp_table:							
		resb PAGE_TBL_SIZE					; Temporarily identity map the kernel
	kernel_stack:
		resb KERNEL_STACK_SIZE				; Reserve memory for stack

section .text                
	align 4
    dd MAGIC_NUMBER  		               	; Write values
    dd FLAGS
    dd CHECKSUM

	extern kmain		; kmain.c : int kmain() 
	
	loader:
		mov esp, kernel_stack + KERNEL_STACK_SIZE	; Initialize stack pointers for function call
		sub esp, kernel_offset						; Keep it in low physical memory
		mov ebp, esp		

		call higher_half_paging						; Higher half map kernel

		; Enable paging
		mov eax, page_directory				
		sub eax, kernel_offset				; This address must be physical
		mov cr3, eax
		mov eax, cr0
		or  eax, 0x80000001					; Set paging and protection bits in CR0
		mov cr0, eax

		mov ecx, paging_enabled				; Do an absolute jump to the (virtual) address of paging_enabled
		lea eax, [ecx]
		jmp eax

	paging_enabled:

		mov esp, kernel_stack + KERNEL_STACK_SIZE	; Paging is now enabled so this address will now point to low physical memory
		mov ebp, esp

		; Remove the identity mapping of low memory
		mov eax, 0x0000000A
		mov [page_directory], eax
		invlpg [0]					; Invalidate it in the cache
		; Release the page frame for the page table

		mov ebx, gdt_start
		push ebx
		mov ebx, idt_start
		push ebx
		call kmain
		pop ebx
		pop ebx

		.loop:                                  		; Idle forever
		    jmp .loop 


	;
	; Maps the first (0x00000000-0x00400000) -> (0x00000000-0x00400000)+kernel_offset
	;
	higher_half_paging:

		;Fill up all of the directory entries with non-present (empty) page tables
		mov ecx, page_directory + PAGE_DIR_SIZE	; Loop condition
		sub ecx, kernel_offset


		mov ebx, page_directory	 	; Temporarily identity map the kernel
		sub ebx, kernel_offset

		mov eax, identity_temp_table
		sub eax, kernel_offset
		and eax, 0xFFFFF000
		or  eax, 0x0000000B			; Set flags for this table (R/W, supervisor only, write-thru cache, 4KiB pages)

		mov [ebx], eax
		add ebx, 4
		mov eax, 0x0000000A			; Same flags as below, but not present
		.fill_directory:
			mov [ebx], eax
			add ebx, 4
			cmp ebx, ecx
			jne .fill_directory

			; Add the real entry

			;Find the index where it should go
			mov eax, kernel_offset
			shr eax, 22
			mov ebx, 4
			mul bx					; 4 byte entries
			
			mov ebx, page_directory
			sub ebx, kernel_offset
			add ebx, eax				; ebx now has the address where it should go

			;Create the entry in eax
			mov eax, page_table_0
			sub eax, kernel_offset
			and eax, 0xFFFFF000
			or  eax, 0x0000000B			; Set flags for this table (R/W, supervisor only, write-thru cache, 4KiB pages)

			;Write change
			mov [ebx], eax
			
			;Now set up the page table that we just pointed to
			mov ecx, page_table_0 + PAGE_TBL_SIZE	; Loop condition
			sub ecx, kernel_offset

			mov eax, 0x0000000B			; Same flag settings as the directory table
			mov ebx, page_table_0
			sub ebx, kernel_offset
		.fill_table_0:
			mov [ebx], eax
			add eax, 0x1000					; Next 4K aligned address
			add ebx, 4
			cmp ebx, ecx
			jne .fill_table_0

			; Set up the identity table
			mov ecx, identity_temp_table + PAGE_TBL_SIZE	; Loop condition
			sub ecx, kernel_offset

			mov eax, 0x0000000B
			mov ebx, identity_temp_table
			sub ebx, kernel_offset
		.fill_temp_table:
			mov [ebx], eax
			add eax, 0x1000					; Next 4K aligned address
			add ebx, 4
			cmp ebx, ecx
			jne .fill_temp_table

			ret
		      
