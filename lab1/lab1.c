#include <syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


#define __NR_newSystemcall 441

struct data_segment{
	unsigned long start_code;
    	unsigned long end_code;
};

int main(){
	struct data_segment my_data_segment;
	printf("Hello, this is lab1\n");
	pid_t pid=getpid();
	printf("Pid is: %d\n", pid);
    	int a = syscall(__NR_newSystemcall, pid, (void*)&my_data_segment);
	printf("system call return value is: %d\n", a);
    	printf("Start: %lx\nEnd: %lx\n", my_data_segment.start_code, my_data_segment.end_code);
	//while(1){};
    	return 0;
}
