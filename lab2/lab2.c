#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <syscall.h>
#include <dlfcn.h>


#define __NR_getPhyAddr 442

/*
struct data_segment{
    unsigned long start_code;
    unsigned long end_code;
};
*/

// data
static int data = 1;       // 儲存於 data 區段（可讀寫區域）
//bss
static int bss_address;           // 儲存於 bss 區段

// TLS
__thread int i = 0;

unsigned long result[7];

char result_name[7][100] = { "code", "data", "BSS", "heap", "libraries", "stack", "thread_local_storages"};
// 子執行緒函數
void* child() {
    //library
    
    void *fHandle;
    void (*func)();


    fHandle = dlopen("./libso.so",RTLD_LAZY);
    if (!fHandle) {
        fprintf (stderr, "%s\n", dlerror());
        exit(1);
    }
    dlerror();
    func = (void(*)())dlsym(fHandle,"bejo_lib");
    
    //heap
    char *heap_address=NULL;
    heap_address = (char *) malloc(15);
    
    //stack
    int stack_address=1;             // 儲存於 stack 區段
    i++;


    unsigned long arr[7] = {&child, &data, &bss_address, heap_address, func, &stack_address, &i};
    
    int a = syscall(__NR_getPhyAddr, arr,7, result, 7);
    
    if(a < 0){
        perror("Sorry,Your system call appears to have failed.");
    }
    else{
    	for(int i=0; i<7; ++i){
    	    printf("virt %s : %#lx\n\n", result_name[i], arr[i]);
    	    printf("phys %s : %#lx\n\n", result_name[i], result[i]);
    	}
    	printf("-------------------------------------\n");
    }
    //free(heap_address);
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
