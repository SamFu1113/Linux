#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <syscall.h>



#define __NR_newSystemcall 443

struct data_segment{
	unsigned long start_code;
    unsigned long end_code;
    unsigned long test_1;
};

// 計數器
__thread int i = 0;

// 子執行緒函數
void* child() {
    struct data_segment my_data_segment;
    pid_t pid=getpid();
    int a = syscall(__NR_newSystemcall, pid, (void*)&my_data_segment);
    i++;
    if(a < 0)
    {
        perror("Sorry,Your system call appears to have failed.");
    }
    else
    {
        printf("child i address %p val %d\n", &i, i);
        //virt_to_phy(&i)
        printf("Congratulations! Your system call is functional. Run the command dmesg in the terminal and find out!\n");
        printf("Start: %lx\nEnd: %lx\n", my_data_segment.start_code, my_data_segment.end_code);
    }
    pthread_exit(NULL);
}

// 主程式
int main() {
  pthread_t t1, t2, t3;
  pthread_create(&t1, NULL, child, NULL);
  pthread_create(&t2, NULL, child, NULL);
  pthread_create(&t3, NULL, child, NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  return 0;
}
