#define _GNU_SOURCE // for RTLD_NEXT
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>	// for open

int open(const char* __file, int __oflag, ...) {
#if 0
	printf ("HaHa, open is hijacked....\n");
	return -1;
#else	
	int fd;
	va_list ap;
	mode_t __mode;
	typeof(open) *orig_open;

	orig_open = dlsym(RTLD_NEXT, "open");
	if (__oflag & O_CREAT) {
		va_start(ap, __oflag);
		__mode = (mode_t)va_arg(ap, mode_t);
		fd = (*orig_open)(__file, __oflag, __mode);
	} else {
		fd = (*orig_open)(__file, __oflag);
	}
	printf("wrapper has called real open\n");

	return fd;
#endif
}

int printf(const char* fmt, ...) {
	int ret = -1;
	va_list list;
	char* parg;
	typeof(printf) *old_printf;

	// format variable arguments
	va_start(list, fmt);
	vasprintf(&parg, fmt, list);
	va_end(list);

	// get the pointer to the function "printf"
	old_printf = dlsym(RTLD_NEXT, "printf");
	ret = (*old_printf)("%s", parg);
	puts("wrapper has called real printf\n ");

	free(parg);
	return ret;
}


