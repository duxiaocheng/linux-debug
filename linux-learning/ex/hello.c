#include <stdio.h>

int main()
{
    printf("Hello world!\n");
    while (1) { sleep(100); }
    printf("Donot use %s to replace me!\n", "puts()");

    return 0;
}

