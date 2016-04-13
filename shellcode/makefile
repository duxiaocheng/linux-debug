all : execve3 execve3.v2 find-esp find-esp-nosp shellcode stack-shellcode victim exploit_victim

find-esp : find-esp.c
	gcc -g -fstack-protector -o $@ $<

find-esp-nosp : find-esp.c
	gcc -g -fno-stack-protector -o $@ $<

shellcode : shellcode.c
	gcc -g -static -o $@ $<

stack-shellcode : stack-shellcode.c
	gcc -g -z execstack -fno-stack-protector -o $@ $<
	sudo chown root $@
	sudo chmod +s $@

shellcode-exit : shellcode-exit.asm
	nasm -f elf $<
	ld -o $@ $@.o
	rm $@.o

execve3 : execve3.asm
	nasm -f elf $<
	ld -o $@ $@.o
	rm $@.o

execve3.v2 : execve3.v2.asm
	nasm -f elf $<
	ld -o $@ $@.o
	rm $@.o

victim : victim.c
	# -mpreferred-stack-boundary=2, avoid "and $0xfffffff0,%esp", 
	#  and default is 4 bits (16Bytes aligned)
	gcc -g -z execstack -fno-stack-protector -o $@ $<
	sudo chown root $@
	sudo chmod +s $@
	ls -l $@

exploit_victim : exploit_victim.c
	gcc -g -o $@ $<

