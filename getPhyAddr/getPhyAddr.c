#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/init_task.h>

/*
struct data_segment{
    unsigned long start_code;
    unsigned long end_code;
};
*/
#define MAX_PROC_SIZE 1000
static unsigned long *proc_data[MAX_PROC_SIZE];
static unsigned long vaddr2paddr(unsigned long vaddr){
    pgd_t *pgd=NULL;
    p4d_t *p4d=NULL;
    pud_t *pud=NULL;
    pmd_t *pmd=NULL;
    pte_t *pte=NULL;
    unsigned long paddr = 0;
    unsigned long page_addr = 0;
    unsigned long page_offset = 0;

    pgd = pgd_offset(current->mm, vaddr);
    //printk("pgd_val = 0x%lxn", pgd_val(*pgd));
    //printk("pgd_index = %lun", pgd_index(vaddr));
    if (pgd_none(*pgd)) {
        printk("not mapped in pgdn");
        return -1;
    }
    p4d = p4d_offset(pgd, vaddr);
    if (p4d_none(*p4d) || p4d_bad(*p4d)){
        printk("not mapped in p4dn");
    	return -1;
    }
    pud = pud_offset(p4d, vaddr);
    //printk("pud_val = 0x%lxn", pud_val(*pud));
    if (pud_none(*pud)) {
        printk("not mapped in pudn");
        return -1;
    }

    pmd = pmd_offset(pud, vaddr);
    //printk("pmd_val = 0x%lxn", pmd_val(*pmd));
    //printk("pmd_index = %lun", pmd_index(vaddr));
    if (pmd_none(*pmd)) {
        printk("not mapped in pmdn");
        return -1;
    }

    pte = pte_offset_kernel(pmd, vaddr);
    //printk("pte_val = 0x%lxn", pte_val(*pte));
    //printk("pte_index = %lun", pte_index(vaddr));
    if (pte_none(*pte)) {
        printk("not mapped in pten");
        return -1;
    }

    /* Page frame physical address mechanism | offset */
    page_addr = pte_val(*pte) & PAGE_MASK;
    page_offset = vaddr & ~PAGE_MASK;
    paddr = page_addr | page_offset;
    printk("page_addr = %lx, page_offset = %lxn", page_addr, page_offset);
    printk("vaddr = %lx, paddr = %lxn", vaddr, paddr);

    return paddr;
}

SYSCALL_DEFINE4(getPhyAddr, unsigned long*, initial, int, len_vir, unsigned long*, result, int, len_phy){
	printk("***\n");
	printk("The process is %s (pid %i)\n", current->comm, current->pid);

	if(copy_from_user(proc_data, initial, sizeof(initial)*len_vir)){
		printk("Copy from user failed!\n");
		return -EFAULT;
	}

	printk("Content is %p\n", proc_data[0]);
	printk("Content is %p\n", proc_data[1]);
	printk("Content is %p\n", proc_data[2]);

	unsigned long *phy[len_phy];
	int i=0;
	while(i<len_phy){
		phy[i]=vaddr2paddr(proc_data[i]);
		printk("Phy%i : %p\n", i, phy[i]);
		++i;
	}
	if(copy_to_user(result, &phy, sizeof(result)*len_phy)){
                printk("Copy to user failed!\n");
        }

	printk("***\n");

	return 0;
}
