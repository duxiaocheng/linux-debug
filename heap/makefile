all : heap-debug-memwatch heap-debug-mtrace heap-debug malloc-hook malloc-wrap malloc-stats

heap-debug-memwatch : heap-debug.c
	gcc -g -DMEMWATCH -DMW_STDIO memwatch.c $< -o $@

heap-debug-mtrace : heap-debug.c
	gcc -g -DMTRACE_TEST $< -o $@

heap-debug : heap-debug.c
	gcc -g $< -o $@

malloc-hook : malloc-hook.c
	gcc -g $< -o $@

malloc-wrap : malloc-wrap.c
	gcc -g -Wl,-wrap,malloc -Wl,-wrap,free $< -o $@

malloc-stats : malloc-stats.c
	gcc -g $< -o $@ -pthread -Wall -W


