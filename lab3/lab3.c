#include <stdio.h>
#include <syscall.h>
#include <unistd.h>

#define NUMBER_OF_ITERATIONS 1000

#define __NR_getContextSwitches 443

int main()
{
	int i=0, t = 2, u = 3, v=0;
	pid_t pid = getpid();
	int nvcw=0;

	for (i = 0; i < NUMBER_OF_ITERATIONS; i++)
		v = (++t) * (u++);

	if (syscall(__NR_getContextSwitches, &nvcw))
		printf("Error!\n");
	else
		printf("This process encounters %u times context switches.\n", nvcw);

	// getchar();	// cat /proc/PID/sched to validate the result

	return 0;
}
