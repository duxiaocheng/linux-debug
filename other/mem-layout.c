#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int bssvar; // uninitialized global variable
int datavar = 100;
extern char** environ;

int main(int argc, char** argv) {
	void* ptr;
	void* p;
	static char s_data[] = "hello world!";

	// resource management: ulimit and getrlimit(RLIMIT_STACK, ...)
	// ulimit -s: stack size, 10240 soft limited
	// ulimit -s H: unlimited, hard limited
	// over the stack size, and trigger the segment fault
#if 0
	char buffer[12*1024*1024 + 10];
	memset(buffer, 1, sizeof(buffer));
#endif
	printf("pid: %d\n", getpid());
	printf("main start    = 0x%08x\n", main);
	printf("data start    = 0x%08x, 0x%08x\n", s_data, &datavar);
	printf("bss end       = 0x%08x\n", (long)&bssvar + 4);

	// heap segment, if no heap used, no heap segment
	p = malloc(128);
	printf("heap start    = 0x%08x\n", p);
	// Calling sbrk() with an increment of 0 can be used to 
	// find the current location of the program break.
	// 0950a000-0952b000 rw-p 00000000 00:00 0          [heap]
	ptr = sbrk(0);
	printf("current brk   = 0x%08x\n", (long*)ptr); // 0x0952b000

	// the process environ, and located in stack
	// #cat /proc/3934/maps
	// bfa24000-bfa39000 rw-p 00000000 00:00 0          [stack]
	// environ:0xbfa371bc, environ[0]:0xbfa3831d
	// argv:0xbfa371b4, argv[0]:0xbfa38310
	printf("environ:0x%08x, environ[0]:0x%08x\n", environ, environ[0]);
	printf("argv:0x%08x, argv[0]:0x%08x\n", argv, argv[0]);

	// pause for taking look maps
	printf("press any key to continue ...\n");
	getchar();

	// test segment fault
	int i = 0x08049628; // heap start address
	brk((char*)0x0804A123); // change the program break, and page align with 0x1000
			       // so actually it will segment fault at 0x0804B000
	for (;;i++) {
		printf("At:0x%x-0x%x\n", i, *((char*)i));
		// At:0x804afff-0x0
		// Segmentation fault
	}
}


// echo 0 > /proc/sys/kernel/randomize_va_space
//
