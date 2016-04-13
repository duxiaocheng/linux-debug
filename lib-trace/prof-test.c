#include <stdio.h>

int f1() {
	int i = 0, g = 0;
	while (i < 10000000) {
		g += i;
		i++;
	}
	return g;
}

int f2() {
	int i = 0, g = 0;
	while (i < 40000000) {
		g += i;
		i++;
	}
	return g;
}

int main(int argc, char* argv[]) {
	int n = 1;
	if (argc > 1) {
		n = atoi(argv[1]);
	}
	while (n > 0) {
		f1();
		f2();
		n--;
	}
	return 0;
}

// $make prof-test
// $time ./prof-test
// $gprof -b ./prof-test
//
