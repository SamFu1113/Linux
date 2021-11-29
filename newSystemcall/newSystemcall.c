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


static int mpid=1;

static void print_vma(struct task_struct *task)
{
        struct mm_struct *mm;
        struct vm_area_struct *vma;
        int count=0;
        mm = task->mm;
        printk("This mm_struct has %d vma\n", mm->map_count);
// &i   （virtual add）
// copy_from_user（&i）
        for(vma = mm->mmap; vma; vma=vma->vm_next){
                printk("vma number %d: \n", ++count);
                // copy_to_user
                printk("Start address 0x%lx, End address 0x%lx\n", vma->vm_start, vma->vm_end);
                printk("Start physical address 0x%lx, End physical address 0x%lx\n", virt_to_phys(vma->vm_start), virt_to_phys(vma->vm_end));
        }

        /*
        printk("Code segment start=0x%lx, end=0x%lx\n"
                "Data Segment start=0x%lx, end=0x%lx\n"
                "Stack segment start=0x%lx\n",
                mm->start_code, mm->end_code, mm->start_data, mm->end_data, mm->start_stack
                );
        */
}

// asmlinkage long sys_newSystemcall(pid_t user_pid, void* __user user_address);
// asmlinkage long my_get_physical_addresses(unsigned long* initial, int len_vir, unsigned int* result, int len_phy)

SYSCALL_DEFINE2(newSystemcall, pid_t user_pid, unsigned long *initial){
	struct task_struct *task_list;
	struct data_segment my_data_segment;
	int ret=0, count=0;

	printk("Hello, this is lab2. This lab will add a new system call.\n");
	printk("Pid we want is: %d\n", user_pid);


	//for_each_process(task_list) {
		//if(task_list->pid == user_pid){
	printk("%s[%d]\n", current->comm, current->pid);
	print_vma(current);
		

	for(int i = 0; i< 3; ++i){
		printk(virt_to_phys(initial[i]));
	}

	if(ret!=0){
		printk("Copy to user space failed!\n");
	}

	printk("\n\n");
	return 0;
}
