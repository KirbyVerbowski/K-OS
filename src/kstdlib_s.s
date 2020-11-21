global write_fb_cell			; void write_fb_cell(unsigned short int xpos, 
								; 					 unsigned short int ypos, 
								;					 char character,
								;					 unsigned char bg,
								;					 unsigned char fg);
								
global set_fb_cursor			; void set_fb_cursor(unsigned short int xpos,
								; 						unsigned short int ypos);
								
global get_fb_cursor			; void get_fb_cursor(void);

FRAMEBUFFER		equ 0x000B8000	; Location of the framebuffer in memory
CELL_SIZE		equ 2			; Framebuffer memory is split into 16-bit cells
LINE_WIDTH		equ 160			; 80 columns in the framebuffer
LINE_COUNT		equ 25			; 25 rows
FB_DATA_PORT	equ 0x03D5		; Memory-mapped I/O data port address
FB_DESC_PORT	equ 0x03D4		; Memory-mapped I/O data descriptor port address
CURS_POS_H		equ 0x0E		; Descriptor for sending high byte of cursor position
CURS_POS_L		equ 0x0F		;

section .text
	write_fb_cell:
		
		; Check parameter validity, don't want to write to non-fb memory
		
		mov  ebx, [esp + 4] 	; Fetch x
		cmp	 ebx, LINE_WIDTH/2	; Check x < 80.	
		ja	 err_arg_x
		cmp	 ebx, 0				; Check x >= 0
		jnae err_arg_x
		
		mov  eax, [esp + 8] 	; Fetch y
		cmp	 eax, LINE_COUNT	; Check y < 25.	
		ja	 err_arg_y
		cmp	 eax, 0				; Check y >= 0
		jnae err_arg_y	
		
		mov  edi, LINE_WIDTH
		mul  edi			; Get the base index
		mov  ecx, eax		; Store result in ecx for later
		
		mov  edx, 0			; Clear edx- will hold encoded value
		mov  dh, [esp + 12]	; Put the ascii value into the high byte of dx
							; edx is now 0x0000XX00
		
		mov  esi, [esp + 16]; Fetch bg
		and  esi, 0x0000000F; Grab the low nibble of bg
		shl  esi, 4			; Put it where it needs to be. 	esi is now 0x000000X0
		
		or   edx, esi		; edx is now 0x0000XXX0
		
		mov  esi, [esp + 20]; Fetch fg
		and  esi, 0x0000000F; Grab low nibble of fg
		
		or   edx, esi		; Encoded character and color data is now in dx
		
		mov byte [ecx + ebx*CELL_SIZE + FRAMEBUFFER], dh	;Write the value out like so to avoid small-endian writing
		mov byte [ecx + ebx*CELL_SIZE + FRAMEBUFFER+1], dl
		
		mov  eax, 0			; No errors- will be returning 0
		jmp cleanup
		
		
	set_fb_cursor:
		; Check parameter validity, don't want to write to non-fb memory
		
		mov  ebx, [esp + 4] 	; Fetch x
		cmp	 ebx, LINE_WIDTH/2	; Check x < 80.	
		ja	 err_arg_x
		cmp	 ebx, 0				; Check x >= 0
		jnae err_arg_x
		
		mov  eax, [esp + 8] 	; Fetch y
		cmp	 eax, LINE_COUNT	; Check y < 25.	
		ja	 err_arg_y
		cmp	 eax, 0				; Check y >= 0
		jnae err_arg_y	
		
		mov  edi, LINE_WIDTH/2
		mul  edi				
		add  eax, ebx			; POS = (y * COLS) + x
		
		mov  ecx, eax			; Put POS in ecx- eax is needed
		
		;put port address in edx, value in al
		mov  dx, FB_DESC_PORT
		mov  al, CURS_POS_H
		out  dx, al
		
		mov  dx, FB_DATA_PORT
		mov  al, ch
		out  dx, al
		
		mov  dx, FB_DESC_PORT
		mov  al, CURS_POS_L
		out  dx, al
		
		mov  dx, FB_DATA_PORT
		mov  al, cl
		out  dx, al
		
		mov eax, 0				; No errors- return 0
		jmp cleanup
		
	get_fb_cursor:
		mov  dx, FB_DESC_PORT	; Set data port to high cursor byte
		mov  al, CURS_POS_H
		out  dx, al
		
		mov  dx, FB_DATA_PORT	; Grab the high byte
		in   al, dx
		shl  ax, 4
		mov  bx, ax				; Temporarily store in bx
		
		mov  dx, FB_DESC_PORT	; Set data port to low cursor byte
		mov  al, CURS_POS_L
		out  dx, al			
		
		mov  dx, FB_DATA_PORT	
		mov  ax, bx				; Restore the high cursor byte into ax
		in   al, dx				; Finally get the low cursor byte in al
		
		ret
		
	cleanup:
		ret			
		
	err_arg_x:
		mov  eax, 1
		jmp  cleanup
	err_arg_y:
		mov  eax, 2
		jmp  cleanup
		
		

		
		
	
