#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHUNKSZ 16*1024*1024

int main()
{
	void* p = NULL;
	int count = 0;
	pid_t pid = getpid();
	char cmd[128] = {0};

	printf("pid is %d\n", pid);
	snprintf(cmd, sizeof(cmd) - 1, 
		"cat /proc/%d/status | grep -E '(VmRSS|VmSize)' | awk '{printf(\"%%s\t\", $0) }'", pid);
	while (1)
	{
		p = malloc(CHUNKSZ);
		if (!p)
		{
			perror("malloc");
			exit(-1);
		}
#ifdef OOM_TEST
		memset(p, 1, CHUNKSZ);
#endif
		sleep(1);
		count++;
		printf("currently allocating %d kB\n", count*16);
		system(cmd);
	}
	//getchar();
	return 0;
}

