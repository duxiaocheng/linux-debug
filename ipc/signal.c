#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>

#define STATIC_ASSERT(x) ((void)sizeof(char[(x)?1:-1]))

static void sig_usr(int sig)
{
	STATIC_ASSERT(SIGUSR1 == 10);
	STATIC_ASSERT(SIGUSR2 == 12);
	printf("Process %d, %d:%s\n", getpid(), sig, strsignal(sig));
}

static void sig_handler(int sig, siginfo_t* info, void* act)
{
	printf("Process %d, %d:%s\n", getpid(), sig, strsignal(sig));
	printf("the ptr value is %s\n", (char*)info->si_ptr);
}

int main()
{
	// case 1:
	printf("=============case 1:\n");
	struct sigaction act;
        act.sa_flags = SA_SIGINFO;
        act.sa_sigaction = sig_handler;
        sigemptyset(&act.sa_mask);
        sigaction(SIGUSR1, &act, NULL);

	union sigval mysigval;
	mysigval.sival_ptr = "Hello world!";
	sigqueue(getpid(), SIGUSR1, mysigval);
	sleep(2); // wait for the signal

	// case 2: 
	printf("=============case 2:\n");
	// typedef void (*sighandler_t)(int)；
	// sighandler_t signal(int signum, sighandler_t handler));
	signal(SIGUSR1, sig_usr);
	signal(SIGUSR2, sig_usr);

	pid_t pid = fork();
	if (pid > 0)
	{
		printf("I am parent process: %d\n", getpid());
		kill(pid, SIGUSR1);
		// parent process waits child process exit
		while (wait(NULL) < 0)
		{
			;
		}
	}
	else
	{
		printf("I am child process: %d\n", getpid());
		// child process sends a signal to parent process
		sleep(2);
		kill(getppid(), SIGUSR1);
		kill(getppid(), SIGUSR1); // parent process maybe ignore this signal
					  // or not, it is not defined
		sleep(2);
		kill(getppid(), SIGUSR2);
		exit(0);
	}

	return 0;
}

