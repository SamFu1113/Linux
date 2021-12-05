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
//code segments, data segments, BSS segments, heap segments, libraries, stack segments, and thread local storages
unsigned char result_name[7][100] = {"code", "data", "BSS", "heap", "libraries", "stack", "thread local storages"};

// 子執行緒函數
void* child(int num) {
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

    int a = syscall(__NR_getPhyAddr, arr, 7, result, 7);

    if(a < 0){
        perror("Sorry,Your system call appears to have failed.");
    }
    else{
        printf("thread %d.\n", num);
        printf("virtual--> %s %#lx , %s %#lx , %s %#lx , %s %#lx , %s %#lx , %s %#lx , %s %#lx \n\n",
        result_name[0], arr[0],
        result_name[1], arr[1],
        result_name[2], arr[2],
        result_name[3], arr[3],
        result_name[4], arr[4],
        result_name[5], arr[5],
        result_name[6], arr[6]
        );
        printf("physcial--> %s %#lx , %s %#lx , %s %#lx , %s %#lx , %s %#lx , %s %#lx , %s %#lx \n\n",
        result_name[0], result[0],
        result_name[1], result[1],
        result_name[2], result[2],
        result_name[3], result[3],
        result_name[4], result[4],
        result_name[5], result[5],
        result_name[6], result[6]
        );
    	printf("-------------------------------------\n");
    }
    free(heap_address);
    pthread_exit(NULL);
}

// 主程式
int main() {
    int cur_question = 0;
    printf("Type 1 for q1, 2 for q2.\n");
    scanf("%d\n", &cur_question);
    if (cur_question == 1) {
      pthread_t t1, t2, t3;
      pthread_create(&t1, NULL, child, 1);
      pthread_create(&t2, NULL, child, 2);
      pthread_create(&t3, NULL, child, 3);
      pthread_join(t1, NULL);
      pthread_join(t2, NULL);
      pthread_join(t3, NULL);
      return 0;
    }
    else if(cur_question==2){
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

        unsigned long arr[7] = {&main, &data, &bss_address, heap_address, func, &stack_address, &i};

        int a = syscall(__NR_getPhyAddr, arr,7, result, 7);

        if(a < 0){
            perror("Sorry,Your system call appears to have failed.");
        }
        else{
            printf("virtual--> %s %#lx , %s %#lx , %s %#lx , %s %#lx , %s %#lx , %s %#lx , %s %#lx \n\n",result_name[0], arr[0], result_name[1], arr[1],
            result_name[2], arr[2],
            result_name[3], arr[3],
            result_name[4], arr[4],
            result_name[5], arr[5],
            result_name[6], arr[6]);
            printf("physical--> %s %#lx , %s %#lx , %s %#lx , %s %#lx , %s %#lx , %s %#lx , %s %#lx \n\n",
            result_name[0], result[0],
            result_name[1], result[1],
            result_name[2], result[2],
            result_name[3], result[3],
            result_name[4], result[4],
            result_name[5], result[5],
            result_name[6], result[6]
            );
            printf("-------------------------------------\n");
        }

        while(1){

        }
    }

}
