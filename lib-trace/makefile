all : preload-test.so libmy_wrap.so get-ip prof-test

preload-test.so : preload-test.c
	gcc -c -fPIC -o preload-test.o $<
	gcc -shared -Wl,-soname,preload-test.so.1 -o $@ preload-test.o -lc 
	rm preload-test.o

libmy_wrap.so : libmy_wrap.c
	gcc -c -fPIC -o libmy_wrap.o $<
	gcc -shared -Wl,-soname,libmy_wrap.so.1 -o $@ libmy_wrap.o -lc -ldl
	rm libmy_wrap.o
	gcc -o test_wrap test_wrap.c

get-ip : get-ip.c
	gcc -o $@ $<

prof-test : prof-test.c
	gcc -pg -o $@ $<

