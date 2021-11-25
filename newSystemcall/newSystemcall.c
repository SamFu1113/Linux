#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/init_task.h>
#include <linux/io.h>

struct data_segment{
    unsigned long start_code;
    unsigned long end_code;
};


SYSCALL_DEFINE2(newSystemcall, pid_t, user_pid, void* __user, user_address){
	struct task_struct *task_list;
	struct data_segment my_data_segment;
	int ret=0, count=0;
	
	printk("Hello, this is lab2. This lab will add a new system call.\n");
	printk("Pid we want is: %d\n", user_pid);
	for_each_process(task_list){
		printk("Task list pid: %d\n", task_list->pid);
		if(task_list->pid == user_pid){
			printk("task pid==user pid\n");
            my_data_segment.start_code = task_list->mm->start_code;
			printk("after start\n");
            my_data_segment.end_code = task_list->mm->end_code;

            ret = copy_to_user(user_address, &my_data_segment, sizeof(struct data_segment));
			if(ret!=0){
				printk("Copy to user space failed!\n");
			}
        }
		++count;
		printk("Executed: %d times\n", count);
		if(count==1000){
			return 0;
		}
	}
	printk("end!\n");
	printk("start code: %lx\n", my_data_segment.start_code);
	printk("physical start code: %lx\n", virt_to_phys(my_data_segment.start_code));
	printk("end code: %lx\n ", my_data_segment.end_code);
    printk("physical end code: %lx\n", virt_to_phys(my_data_segment.end_code));

	return 0;
}
