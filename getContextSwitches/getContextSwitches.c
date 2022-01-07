#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/resource.h>
#include <asm/errno.h>


// asmlinkage long sys_getContextSwitches(pid_t user_pid, unsigned int* number_of_context_switches);

SYSCALL_DEFINE1(getContextSwitches,  unsigned int *, number_of_context_switches) {
    struct task_struct *task;
    struct rusage ru;
    unsigned int ncsw;
    task = current
    //task = find_task_by_vpid(current->pid);
    if (!task) {
        return -ESRCH;
    }

    getrusage(task, RUSAGE_SELF, &ru);

    printk("---\n");
    printk("task->nvcsw: %ld\n", task->nvcsw);
    printk("task->nivcsw: %ld\n", task->nivcsw);
    printk("task->signal->nvcsw: %ld\n", task->signal->nvcsw);
    printk("task->signal->nivcsw: %ld\n", task->signal->nivcsw);
    printk("task->last_switch_count: %ld\n", task->last_switch_count);
    printk("ru.ru_nvcsw: %ld\n", ru.ru_nvcsw);
    printk("ru.ru_nivcsw: %ld\n", ru.ru_nivcsw);

    ncsw = ru.ru_nvcsw + ru.ru_nivcsw;
    if(copy_to_user(number_of_context_switches, &ncsw, sizeof(unsigned int))) {
        return -EFAULT;
    }

    return 0;
}
