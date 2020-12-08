ENTRY(loader)                

SECTIONS {
	grub_size = 0x00100000;
	kernel_offset = 0xC0000000;

    . = kernel_offset + grub_size;          /* load code at 1 MB + 3GB*/


	kernel_virtual_start = .;
	kernel_physical_start = . - kernel_offset;

    .text ALIGN (0x1000) : AT (ADDR (.text) - kernel_offset)  /* align at 4 KB */
    {
        *(.text)             /* all text sections from all files */
    }

    .rodata ALIGN (0x1000) : AT (ADDR (.rodata) - kernel_offset) /* align at 4 KB */
    {
        *(.rodata*)          /* all read-only data sections from all files */
    }

    .data ALIGN (0x1000) :  AT (ADDR (.data) - kernel_offset) /* align at 4 KB */
    {
        *(.data)             /* all data sections from all files */
    }

    .bss ALIGN (0x1000) :  AT (ADDR (.bss) - kernel_offset)  /* align at 4 KB */
    {
        *(COMMON)            /* all COMMON sections from all files */
        *(.bss)              /* all bss sections from all files */
    }

	kernel_virtual_end = .;
	kernel_physical_end = . - kernel_offset;
}