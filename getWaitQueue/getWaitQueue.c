#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/resource.h>
#include <asm/errno.h>

SYSCALL_DEFINE2(getWaitQueue, pid_t, pid, unsigned int *, number_of_entering_a_wait_queue)
{
    struct task_struct *task;
    unsigned int newq;

    task = find_task_by_vpid(pid);
    if (!task)
    {
        return -ESRCH;
    }

    printk("---\n");
    printk("task->enter_queue_count: %d\n", task->enter_queue_count);
    newq = task->enter_queue_count;


    if (copy_to_user(number_of_entering_a_wait_queue, &newq, sizeof(unsigned int)))
    {
        return -EFAULT;
    }

    return 0;
}
