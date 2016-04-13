all : stack-example stack-example-FPO call-convention stack-thread stack-overflow stack-jump

stack-example : stack-example.c
	gcc -S -o $@.s $<
	gcc -g -o $@ $<

stack-example-FPO : stack-example.c
	gcc -fomit-frame-pointer -S -o $@.s $<
	gcc -fomit-frame-pointer -g -o $@ $<

call-convention : call-convention.c
	gcc -g -o $@ $< -Wall -W

stack-thread : stack-thread.c
	gcc -g -o $@ $< -pthread -Wall -W

stack-overflow : stack-overflow.c
	gcc -S -o $@.s $<
	gcc -g -o $@ $<

stack-overflow-sp : stack-overflow.c
	gcc -S -fstack-protector -o $@.s $<
	gcc -g -fstack-protector -o $@ $<

stack-jump : stack-jump.c
	gcc -S -o $@.s $<
	gcc -g -o $@ $<

