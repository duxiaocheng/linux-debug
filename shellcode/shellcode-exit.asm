Section .text
	global _start

; exit(0)
_start:
	;mov ebx,0	;argument
	;mov eax,1	;system call number for exit
	xor ebx,ebx
	mov al,1
	int 0x80

; $ nasm -f elf shellcode-exit.asm  
; $ ld -o shellcode shellcode-exit.o   
; $ objdump -d shellcode-exit

