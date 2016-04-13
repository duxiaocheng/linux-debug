#include <stdio.h>
#include <string.h>

unsigned long find_esp() {
	__asm__("movl %esp, %eax");
}

int main(int argc, char** argv) {
	printf("0x%x\n", find_esp());
}

// /proc/sys/kernel/randomize_va_space = [0|1|2]
// if this value is 0, the esp is fixed.
