#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define __NR_mycall 440

long helloworld_syscall(void)
{
    return syscall(__NR_mycall);
}

int main(int argc, char *argv[])
{
    long activity;
    activity = helloworld_syscall();

    if(activity < 0)
    {
        perror("Sorry, Sam. Your system call appears to have failed.");
    }

    else
    {
        printf("Congratulations, Sam! Your system call is functional. Run the command dmesg in the terminal and find out!\n");
    }

    return 0;
}
