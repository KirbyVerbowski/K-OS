global loader

    MAGIC_NUMBER    equ 0x1BADB002      ; GRUB looks for this number
    FLAGS           equ 0x0     
    CHECKSUM        equ -MAGIC_NUMBER

    section .text:                  
    align 4
        dd MAGIC_NUMBER                 ; Write values to machine code
        dd FLAGS
        dd CHECKSUM

    loader:
        mov eax, 0xFADEDDAB             ; Funny number for verification
    .loop:                              ; No further instructions
        jmp .loop       
