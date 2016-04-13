#include <stdio.h>

int __attribute__((__cdecl__)) add1(int a, int b)
{
	int c = a + b;
	return c;
}

int __attribute__((__stdcall__)) add2(int a, int b)
{
	int c = a + b;
	return c;
}

int main()
{
	int a = 0, b = 0;
	a = add1(1, 2); 
	b = add2(3, 4); 
	return 0;
}

/*****************************************************************************
$ objdum -dS call-convention # disassemble with source code, and also gdb disassemble /m main

08048394 <add1>:
#include <stdio.h>
int __attribute__((__cdecl__)) add1(int a, int b)
{
 8048394:	55						push   %ebp
 8048395:	89 e5					mov    %esp,%ebp
 8048397:	83 ec 10				sub    $0x10,%esp
	int c = a + b;
 804839a:	8b 45 0c				mov    0xc(%ebp),%eax
 804839d:	8b 55 08				mov    0x8(%ebp),%edx
 80483a0:	8d 04 02				lea    (%edx,%eax,1),%eax
 80483a3:	89 45 fc				mov    %eax,-0x4(%ebp)
	return c;
 80483a6:	8b 45 fc				mov    -0x4(%ebp),%eax
}
80483a9:	c9						leave  
80483aa:	c3						ret       =============> caller clear the argument stack

080483ab <add2>:
int __attribute__((__stdcall__)) add2(int a, int b)
{
 80483ab:	55						push   %ebp
 80483ac:	89 e5					mov    %esp,%ebp
 80483ae:	83 ec 10				sub    $0x10,%esp
	int c = a + b;
 80483b1:	8b 45 0c				mov    0xc(%ebp),%eax
 80483b4:	8b 55 08				mov    0x8(%ebp),%edx
 80483b7:	8d 04 02				lea    (%edx,%eax,1),%eax
 80483ba:	89 45 fc				mov    %eax,-0x4(%ebp)
	return c;
 80483bd:	8b 45 fc				mov    -0x4(%ebp),%eax
}
80483c0:	c9						leave  
80483c1:	c2 08 00				ret    $0x8 ===========> callee clear the argument stack
                                                ***********>  pop EIP; add ESP, $08

080483c4 <main>:
int main()
{
 80483c4:	55						push   %ebp
 80483c5:	89 e5					mov    %esp,%ebp
 80483c7:	83 ec 18				sub    $0x18,%esp
	int a = 0, b = 0;
 80483ca:	c7 45 f8 00 00 00 00	movl   $0x0,-0x8(%ebp)
 80483d1:	c7 45 fc 00 00 00 00	movl   $0x0,-0x4(%ebp)
	a = add1(1, 2); 
 80483d8:	c7 44 24 04 02 00 00	movl   $0x2,0x4(%esp)    ==> push $2
 80483df:	00 
 80483e0:	c7 04 24 01 00 00 00	movl   $0x1,(%esp)       ==> push $1
 80483e7:	e8 a8 ff ff ff			call   8048394 <add1>    ==> push EIP and jmp
 80483ec:	89 45 f8				mov    %eax,-0x8(%ebp)   ==> get return value
	b = add2(3, 4); 
 80483ef:	c7 44 24 04 04 00 00	movl   $0x4,0x4(%esp)    ==> push $4
 80483f6:	00 
 80483f7:	c7 04 24 03 00 00 00	movl   $0x3,(%esp)       ==> push $3
 80483fe:	e8 a8 ff ff ff			call   80483ab <add2>    ==> push EIP and jmp
 8048403:	83 ec 08				sub    $0x8,%esp ******* ==> restore the ESP
 8048406:	89 45 fc				mov    %eax,-0x4(%ebp)   ==> get return value
	return 0;
 8048409:	b8 00 00 00 00			mov    $0x0,%eax
}
804840e:	c9						leave  
804840f:	c3						ret    ====================> caller clear the argument stack
*****************************************************************************/
