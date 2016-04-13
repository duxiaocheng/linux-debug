#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>

void print_mallinfo() {
	printf("----------mallinfo:\n");
	struct mallinfo info = mallinfo();
	// the total size of memory allocated with sbrk by malloc, in bytes.
	printf("arena    = %d\n", info.arena);
	// This is the number of chunks not in use.
	printf("ordblks  = %d\n", info.ordblks);
	printf("smblks   = %d\n", info.smblks);
	printf("hblks    = %d\n", info.hblks);
	printf("hblkhd   = %d\n", info.hblkhd);
	printf("usmblks  = %d\n", info.usmblks);
	printf("fsmblks  = %d\n", info.fsmblks);
	// the total size of memory occupied by chunks handed out by malloc.
	printf("uordblks = %d\n", info.uordblks);
	// the total size of memory occupied by free (not in use) chunks.
	printf("fordblks = %d\n", info.fordblks);
	// the size of the top-most releasable chunk that 
	// normally borders the end of the heap
	printf("keepcost = %d\n", info.keepcost);
}


void function() {
	malloc(100);
}

int test() {
	printf("==========before call function\n");
	malloc_stats();
	print_mallinfo();
	malloc_info(0, stdout);
	// per thread ???
	// used to test a function if has memory leak
	// before: Arena 0: in use bytes = 0, mallinfo().uordblks = 0
	// after : Arena 0: in use bytes = 104, mallinfo().uordblks = 104
	// note: this is the number of bytes allocated, not the number requested.
	// so this function has about 100 bytes memory leak
	function();
	printf("==========after cal function\n");
	malloc_stats();
	print_mallinfo();
	malloc_info(0, stdout);

	return 0;
}

static void* thread_process(void* arg)
{
	test();
	return NULL;
}

int main()
{
	test();

	pthread_t tid;
	pthread_create(&tid, NULL, &thread_process, NULL);
	pthread_join(tid, NULL);

	return 0;
}

// ==========before call function
// Arena 0:                           // thread 0
// system bytes     =          0      // allocation size from system     
// in use bytes     =          0      // in used memory size
// Total (incl. mmap):
// system bytes     =          0
// in use bytes     =          0
// max mmap regions =          0
// max mmap bytes   =          0
// ==========after cal function
// Arena 0:
// system bytes     =     135168
// in use bytes     =        104
// Total (incl. mmap):
// system bytes     =     135168
// in use bytes     =        104
// max mmap regions =          0
// max mmap bytes   =          0
//
