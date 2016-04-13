#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* If we're not using GNU C, elide __attribute__ */
#ifndef __GNUC__
#  define  __attribute__(x)  /*NOTHING*/
#endif

#define JOIN(a, b) JOIN_DO(a, b)
#define JOIN_DO(a, b) a##b

#if 0
#define MY_STATIC_ASSERT(condition) ((void)sizeof(char[(!!(condition))?1:-1]))
#else
#define MY_STATIC_ASSERT(x) typedef char JOIN(static_assert_, __LINE__)[(x)?1:-1]
#endif

// typedef char static_assert_20[1];
MY_STATIC_ASSERT(sizeof(int) == 4);

int add(int a, int b)
{
	// warning: control reaches end of non-void function
	// if compile it with -O option, this code will be ignored
	int c = a + b; (void)c;
	//printf("%s:%d, %d + %d = %d\n", __FUNCTION__, __LINE__, a, b, c);
}

//__attribute__((format(printf, 1, 2)))
int my_printf(const char* fmt, ...)
{
	char buffer[128] = {0};
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buffer, sizeof(buffer) - 1, fmt, ap);
	va_end(ap);
	return printf(buffer);
}

// This attribute marks the function as considering only its numeric parameters. 
// This is mainly intended for the compiler to optimize away repeated calls to a 
// function that the compiler knows will return the same value repeatedly. 
// It applies mostly to math functions that have no static state or side effects, 
// and whose return is solely determined by the inputs. 
// ========gcc -O and this function is called one time
__attribute__((const)) int inc(int x)
{
	printf("%s(%d)\n", __FUNCTION__, x);
	return x + 1;
}

void align()
{
	struct {
		int i;
		short s;
		char c;
	} normal_stu;
	MY_STATIC_ASSERT(sizeof(normal_stu) == 8);
	printf("sizeof normal struct:%d\n", sizeof(normal_stu));

	struct {
		int i;
		short s;
		char c;
	} __attribute__((aligned(16))) aligned_stu;
	MY_STATIC_ASSERT(sizeof(aligned_stu) == 16);
	printf("sizeof aligned struct:%d\n", sizeof(aligned_stu));

	struct {
		int i;
		short s;
		char c;
	} __attribute__((__packed__)) packed_stu;
	MY_STATIC_ASSERT(sizeof(packed_stu) == 7);
	printf("sizeof packed struct:%d\n", sizeof(packed_stu));
}

__attribute__((constructor(101))) void before() 
{
	printf("before main(): hello world!\n");
}

int main(int argc, char* argv[])
{
#ifdef NDEBUG
	// warning: #warning ("Defined NDEBUF")
#	warning("Defined NDEBUF")
#endif

	// warning: unused variable ‘unused_var’
	int unused_var = 0;
	// (void)unused_var;

	// warning: too few arguments for format
	// segmentation fault maybe generated here, since %s will read a address, 
	// if this address is invalid, then error occur 
	printf("argc = %d, argv = %s\n", argc);
	my_printf("argc = %d, argv = %s\n", argc); // no warning

	struct s { int f, g, h; };
	// warning: missing initializer
	struct s x = { 3, 4 }; (void)x;
	struct s x2 = {.f = 3, .g = 4}; (void)x2; // no warning

	int a = 5, b = 4;
	int c = add(a, b);
	printf("%s:%d, %d + %d = %d\n", __FUNCTION__, __LINE__, a, b, c);

	int i, j, m;
	i = inc(10);
	j = inc(10);
	m = inc(11);
	printf("%d %d %d\n", i, j, m);

	// test aligned attribute
	align();

	return 0;
}

// gcc -Wall

// why no return is also right?
// int c = a + b;
// 80483ca:	8b 45 0c				mov    0xc(%ebp),%eax
// 80483cd:	8b 55 08				mov    0x8(%ebp),%edx
// 80483d0:	8d 04 02				lea    (%edx,%eax,1),%eax
// 80483d3:	89 45 fc				mov    %eax,-0x4(%ebp)
// Load effect address: lea (%edx,%eax,1),%eax; eax=edx+eax*1
//
