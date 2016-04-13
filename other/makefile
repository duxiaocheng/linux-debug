all : mem-layout mem-exhaust mem-exhaust-oom core-dump static-check

mem-layout : mem-layout.c
	gcc -g -o $@ $<

mem-exhaust : mem-exhaust.c
	gcc -o $@ $< -g -Wall -W

mem-exhaust-oom : mem-exhaust.c
	gcc -o $@ $< -g -Wall -W -DOOM_TEST

core-dump : core-dump.c
	gcc -g -o $@ $<

static-check : static-check.c
	gcc -o $@ $< -Wall -W -DNDEBUG -g
	gcc -o $@.optimize $< -DNDEBUG -g -O
