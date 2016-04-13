#include <stdio.h>
#include <stdlib.h>

void* __wrap_malloc(size_t size) {
	void* p = NULL;
	p = (void*)__real_malloc(size);
	printf("malloc size:%d, address:%p\n", size, p);
	return p;
}

void __wrap_free(void* ptr) {
	printf("free address:%p\n", ptr);
	__real_free(ptr);
}

int main() {
	void* p = malloc(100);
	free(p);
	return 0;
}


