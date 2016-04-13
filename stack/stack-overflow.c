#include <stdio.h>
#include <string.h>

void function(char* str) {
	char buffer[16];

	strcpy(buffer, str);
}

int main() {
	char large_string[256];
	int i;

	for (i = 0; i < 255; i++) {
		large_string[i] = 'A';
	}
	function(large_string);

	return 0;
}

// gcc -S -o stack-example.s stack-example.c 
// (gdb) info registers # get the current stack frame pointer(EBP) 
// 
// bottom of memory								top of memory
//           buffer    sfp   ret   *str
//  <------ [        ][    ][    ][    ]
// top of stack									bottom of stack
//
// (gdb) x/8xb 0xbffff138
// 0xbffff138:	0x68	0xf2	0xff	0xbf	0x24	0x84	0x04	0x08
//                                              ============================
// 0x0804841f <+65>:	call   0x80483c4 <function>
// 0x08048424 <+70>:	mov    $0x0,%eax
// 
// (gdb) x/8xb 0xbffff138
// 0xbffff138:	0x41	0x41	0x41	0x41	0x41	0x41	0x41	0x41
//                                              !!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 
