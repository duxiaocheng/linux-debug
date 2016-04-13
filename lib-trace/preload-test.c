#include <mcheck.h>

void __attribute__((constructor)) my_init() {
	mtrace();
}

void __attribute__((destructor)) my_fini() {
	mutrace();
}

// $export MALLOC_TRACE=./mtrace.log
// $export LD_PRELOAD=./preload-test.so
// $ldd /bin/ls
// $ls
// $more ./mtrace.log
// $unset LD_PRELOAD
// $unset MALLOC_TRACE

