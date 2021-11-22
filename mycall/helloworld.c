#include <linux/kernel.h>
#include <linux/syscalls.h>

//asmlinkage int __x64_sys_helloworld(void)
SYSCALL_DEFINE0(helloworld) {
    printk("I'm SamFu\n");
    return 0;
}
