#include <stdio.h>
#include <string.h>

void function(int a, int b, int c) {
	char buffer1[5];
	char buffer2[10];
	int* ret;

	ret = (int*)(buffer1 + 13); // 13 = sizeof(SFP) + offset(buffer) = 4 + 9
	(*ret) += 8; // 8 = expect ret - normal ret
}

int main() {
	int x = 0;

	x = 0;
	function(1, 2, 3);
	x = 1;
	printf("x is %d\n", x);

	return 0;
}

// gcc -S -o stack-example.s stack-example.c 
// (gdb) info registers # get the current stack frame pointer(EBP) 
// 
// bottom of memory								top of memory
//     buffer2   buffer1  *ret  sfp   ret   a     b     c
// <- [   10B   ][  5B  ][ 4B ][ 4B ][ 4B ][    ][    ][    ]
// top of stack									bottom of stack
//
// 0x08048412 <+48>:	call   0x80483c4 <function>
// 0x08048417 <+53>:	movl   $0x1,0x1c(%esp) # normal ret:0x08048417
// 0x0804841f <+61>:	mov    $0x8048504,%eax # expect ret:0x0804841f
//
