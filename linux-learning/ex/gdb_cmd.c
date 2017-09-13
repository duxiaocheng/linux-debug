#include <stdio.h>

int main()
{
    int i = 100;
    int array[10] = {0, 1, 2, 3, 4, 5, 6, };
    char c = 'a';

    // (gdb) print array@3
    // $6 = {0, 1, 2}

    // (gdb) x/8bx array # 8 bytes with hex format
    // 0x7fffffffe430: 0x00   0x00   0x00   0x00    0x01   0x00   0x00   0x00
    // (gdb) x/4wx array # 4 half word with hex format
    // 0x7fffffffe430: 0x0000  0x0000  0x0001  0x0000
    // (gdb) x/2wd array # 2 word with decimal format
    // 0x7fffffffe430: 0       1

    // (gdb) watch array[3]
    // Hardware watchpoint 2: array[3]
    // (gdb) c
    // Continuing.
    // Hardware watchpoint 2: array[3]
    //
    // Old value = 3
    // New value = 9
    array[3] = 9;

    return 0;
}

