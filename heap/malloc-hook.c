#include <stdio.h>
#include <malloc.h>

// define 2 variable to store the old function pointer
static void* (*old_malloc)(size_t size, const void* caller);
static void (*old_free)(void* ptr, const void* caller);

static void* my_malloc(size_t size, const void* caller);
static void my_free(void* ptr, const void* caller);

static void* my_malloc(size_t size, const void* caller) {
	void* p = NULL;
	// restore for calling the real malloc
	__malloc_hook = old_malloc;
	__free_hook = old_free; // printf uses the free 
	// custom malloc
	p = malloc(size);
	printf("malloc size: %d, address:%x\n", size, p);
	// 
	__malloc_hook = my_malloc;
	__free_hook = my_free;
	return p;
}

static void my_free(void* ptr, const void* caller) {
	// restore
	__free_hook = old_free;
	// custom
	printf("free address: %x\n", ptr);
	free(ptr);
	// 
	__free_hook = my_free;
}


int main() {

	// backup the old function pointer
	old_malloc = __malloc_hook;
	old_free = __free_hook;

	// replace
	__malloc_hook = my_malloc;
	__free_hook = my_free;

	void* p = malloc(100);
	free(p);

	// restore
	__malloc_hook = old_malloc;
	__free_hook = old_free;

	return 0;
}


