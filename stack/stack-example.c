#include <stdio.h>

void function(int a, int b, int c) {
	char buffer1[5] = {0};
	char buffer2[10] = {0};
}

int main() {
	function(1, 2, 3);
	return 0;
}

// gcc -S -o stack-example.s stack-example.c 
// (gdb) info registers # get the current stack frame pointer(EBP) 
// 
// bottom of memory								top of memory
//           buffer2       buffer1   sfp   ret   a     b     c
//  <------ [            ][        ][    ][    ][    ][    ][    ]
// top of stack									bottom of stack
//
// (gdb) x/20xb 0xbffff254 # 20 = sizeof(SFP) + sizeof(RET) + sizeof(arguments)
// 0xbffff254:	0x68	0xf2	0xff	0xbf	0xdd	0x83	0x04	0x08
//              -----------SFP--------------    ------------RET-------------
// 0xbffff25c:	0x01	0x00	0x00	0x00	0x02	0x00	0x00	0x00
//              ------------a---------------    -------------b--------------
// 0xbffff264:	0x03	0x00	0x00	0x00
//              ------------c---------------
// (gdb) x/16xb 0xbffff254 - 16
// 0xbffff244:	0x15	0x00	0x00	0x00	0x00	0x00	0x00	0x00
// 0xbffff24c:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
// (gdb) 
//
