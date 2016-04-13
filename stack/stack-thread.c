// demo source code comes from man pthread_create
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>	// for getopt
#include <errno.h>
#include <ctype.h>	// for toupper
#include <limits.h>	// for pthread_stack_min

#define handle_error_en(en, msg) \
	do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct thread_info { // used as argument to thread_start()
	pthread_t	thread_id;		// ID returned by thread_create()
	int			thread_num;		// application-defined thread #
	char		*argv_string;	// from command-line argument
};

// thread start function: display address near top of our stack,
// and return upper-cased copy of argv_string
static void* thread_start(void* arg)
{
	struct thread_info* tinfo = (struct thread_info*)arg;
	char *uargv, *p;

	printf("thread %d: top of stack near 0x%08x; argv_string=%s\n", 
		tinfo->thread_num, (unsigned int)&p, tinfo->argv_string);

	// strdup = malloc + memcpy, so need free 
	uargv = strdup(tinfo->argv_string);
	if (uargv == NULL)
	{
		handle_error("strdup");
	}
	for (p = uargv; *p != '\0'; p++)
	{
		*p = toupper(*p);
	}
	return uargv;
}

int main(int argc, char* argv[])
{

	// the "-s" option specifies a stack size for our threads
	int stack_size = -1;
	int opt = -1;
	extern char* optarg;
	while ((opt = getopt(argc, argv, "s:")) != -1)
	{
		switch (opt)
		{
			case 's' : 
				stack_size = strtoul(optarg, NULL, 0);
				break;
			default : 
				fprintf(stderr, "Usage: %s [-s stack-size] arg ...\n", 
					argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	// optind is the index in argv of the first argv-element that is not an option.
	// for ./a.out -s 0x100000 hole salut servus, optind is 3
	extern int optind;
	int num_threads = argc - optind;

	// initialize thread creation attributes
	pthread_attr_t attr;
	int s = pthread_attr_init(&attr);
	if (s != 0)
	{
		handle_error_en(s, "pthread_attr_init");
	}
	if (stack_size > 0)
	{
		s = pthread_attr_setstacksize(&attr, stack_size);
		if (s != 0)
		{
			printf("The stack size (%d) is less than PTHREAD_STACK_MIN (%d) bytes.\n"
				"Or it is not a multiple of the system page size (%d).\n", 
				stack_size, PTHREAD_STACK_MIN, getpagesize());
			handle_error_en(s, "pthread_attr_setstacksize");
		}
	}
	size_t stack_size_actual = 0;
	pthread_attr_getstacksize(&attr, &stack_size_actual);
	printf("actual stack size: 0x%x (%d)\n", stack_size_actual, stack_size_actual);

	// allocate memory for pthread_create() arguments
	struct thread_info *tinfo;
	// malloc (num_threads * sizeof) memory
	tinfo = calloc(num_threads, sizeof(struct thread_info));
	if (tinfo == NULL)
	{
		handle_error("calloc");
	}

	// create one thread for each command-line argument
	int tnum = 0;
	for (tnum = 0; tnum < num_threads; tnum++)
	{
		tinfo[tnum].thread_num = tnum + 1;
		tinfo[tnum].argv_string = argv[optind + tnum];

		// the pthread_create() call stores the thread ID into
		// corresponding element of tinfo[]
		s = pthread_create(&tinfo[tnum].thread_id, &attr, &thread_start, &tinfo[tnum]);
		if (s != 0)
		{
			handle_error_en(s, "pthread_create");
		}
	}

	// destroy the thread attributes object, since it is no longer needed
	s = pthread_attr_destroy(&attr);
	if (s != 0)
	{
		handle_error_en(s, "pthread_attr_destroy");
	}

	// now join with each thread, and display its returned value
	void *res;
	for (tnum = 0; tnum < num_threads; tnum++)
	{
		s = pthread_join(tinfo[tnum].thread_id, &res);
		if (s != 0)
		{
			handle_error_en(s, "pthread_join");
		}
		printf("joined with thread %d; returned value was %s\n", 
			tinfo[tnum].thread_num, (char*)res);
		free(res); // free memory allocated by thread
	}

	free(tinfo);
	exit(EXIT_SUCCESS);
}

// $ulimit -s
// 10240   # The stack size limit is 10 MB (0xa00000 bytes)
//
// $./stack-thread a b c
// actual stack size: 0xa00000 (10485760)
// thread 3: top of stack near 0xb6bd9374; argv_string=c # b75da374 - b6bd9374
// thread 2: top of stack near 0xb75da374; argv_string=b # = a01000 = 10244 kB
// thread 1: top of stack near 0xb7fdb374; argv_string=a
// joined with thread 1; returned value was A
// joined with thread 2; returned value was B
// joined with thread 3; returned value was C
//
// $./stack-thread -s 0x100000 a b c
// actual stack size: 0x100000 (1048576)
// thread 3: top of stack near 0xb7dd9374; argv_string=c # b7eda374 - b7dd9374
// thread 2: top of stack near 0xb7eda374; argv_string=b # = 101000 = 1028 kB
// thread 1: top of stack near 0xb7fdb374; argv_string=a
// joined with thread 1; returned value was A
// joined with thread 2; returned value was B
// joined with thread 3; returned value was C
//
