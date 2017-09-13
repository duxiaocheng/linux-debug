#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void create_dump()
{
    if (!fork()) { //sub-process
        abort();
    }
}

static void myfunc(int a)
{
    char *p = NULL;
    create_dump();
    //*p = 0;
}


int main()
{
    printf("This is core test case!\n");
    myfunc(100);
    printf("I will enter while loop, and you can type gcore to produce core file!\n");
    while (1) { sleep(100); }
    printf("That is ok!");

    return 0;
}

