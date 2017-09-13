default : hello main main_s core_test gdb_cmd

hello : hello.c
	gcc -g -o $@ $<

main : main.c
	gcc -c -fPIC foo.c
	gcc -shared -o libfoo.so foo.o
	gcc -o $@ $< -lfoo -L./ -Wl,-rpath=./

main_s : main.c foo.c
	gcc -o $@ main.c foo.c

core_test : core_test.c
	gcc -g -o $@ $^

gdb_cmd : gdb_cmd.c
	gcc -g -o $@ $^
