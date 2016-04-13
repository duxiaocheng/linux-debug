#include <stdio.h>
#include <fcntl.h>

int main(int argc, char*argv[]) {
	int fd;
	fd = open("/tmp/ttt", O_CREAT|O_RDWR, 0666);
	if (fd < 0) {
		perror("open");
	} else {
		// we can use command "ltrace ./test_wrap" to trace this printf
		// and the actual call function is puts. So no wrap.
		printf("open was successful\n");
	}

	printf("hello world! argc: %d\n", argc);
	return 0;
}

// $make libmy_wrap.so
// $export LD_PRELOAD=./libmy_wrap.so
// $./test_wrap
// $unset LD_PRELOAD


