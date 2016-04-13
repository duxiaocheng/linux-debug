#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef MTRACE_TEST
#include <mcheck.h>
#endif

#ifdef MEMWATCH
#include "memwatch.h"
#endif

int main() {
	char *p1 = NULL;
	char *p2 = NULL;

#ifdef MTRACE_TEST
	mtrace();
#endif
	p1 = malloc(200);   /* causes unfreed error */

	p2 = malloc(100);
	p2[-1] = 0;         /* causes underflow error */
	free(p2);
#ifdef MTRACE_TEST
	muntrace();
#endif
	return 0;
}

