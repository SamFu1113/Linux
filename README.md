Add system calls under Linux kernel-5.8.1
============

&nbsp;&nbsp;&nbsp;&nbsp;There are several guides for kernel developers and users. These guides can
be rendered in a number of formats, like HTML and PDF. Please read
Documentation/admin-guide/README.rst first.

&nbsp;&nbsp;&nbsp;&nbsp;In order to build the documentation, use ``make htmldocs`` or
``make pdfdocs``.  The formatted documentation can also be read online at:

    https://www.kernel.org/doc/html/latest/

&nbsp;&nbsp;&nbsp;&nbsp;There are various text files in the Documentation/ subdirectory,
several of them using the Restructured Text markup notation.

&nbsp;&nbsp;&nbsp;&nbsp;Please read the **Documentation/process/changes.rst** file, as it contains the
requirements for building and running the kernel, and information about
the problems which may result by upgrading your kernel.

-------------------------------------------------------------------
## In the linux-5.8.1 directory

```sh
vim linux-5.8.1/Makefile
```

in line 1073
(add newSystemcall/)&nbsp;
kernel/ certs/ mm/ fs/ ipc/ security/ crypto/ block/ newSystemcall/

```sh
vim linux-5.8.1/include/linux/syscalls.h
```

add 
asmlinkage long sys_newSystemcall(pid_t user_pid, void* __user user_address);
before ##endif

```sh
vim linux-5.8.1/arch/x86/entry/syscalls/syscall_64.tbl
```
(ID 440 is for sys_helloworld in my pc, 
441     common  newSystemcall)               sys_newSystemcall


The nproc in my pc is 8.


"lab1" folder is in the home directory.
"newSystemcall" folder is under the linux-5.8.1 directory.


"pics" folder is the execution record.

------------------------------------------------------------------
To do work items:

## Define file

1. Check the kernel version and determine where to add sys call.
```sh
uname -r
```
&nbsp;&nbsp;&nbsp;&nbsp;It should show:

```sh
5.8.1 generic
```

&nbsp;&nbsp;&nbsp;&nbsp;**Anotehr member shows 5.11.0-27-generic.**</br>
&nbsp;&nbsp;&nbsp;&nbsp;**Below shows the execution process of the member.**</br>

2. The kernel is stored in ```/usr/src```.

``` sh
cd /usr/src/linux-headers-5.11.0-27-generic
```

3. Configure the system call table.  
Path： ```/usr/src/linux-headers-5.11.0-27-generic/arch/x86/kernel/```  

```sh
cd /usr/src/linux-headers-5.11.0-27-generic/arch/x86/kernel/
```

4. Add the system call file.  (There is no system call file ```syscall_table_32.S``` in the directory of the member.)  
Therefore, she create a new system call file.
nano syscall_table_32.S
    .long sys mycall    /* yan add row */

5. Define new system call 
Path：/usr/src/linux-headers-5.11.0-27-generic/arch/x86/include/asm/unistd.h
（Difference from kernel-2.6.28:  there is no a file named ```unistd_32.h file```, and the file name is changed to: ```unistd.h```.)
```sh
cd /usr/src/linux-headers-5.11.0-27-generic/arch/x86/include/asm/
```
```sh
nano unistd.h
```

&nbsp;&nbsp;&nbsp;&nbsp;***Explanation***: In the file ```unistd.h```, add the number of system call, 
that is to add ```#define __NR_mycall```, and it is defined as the 333rd system call number, it is ```__NR_syscall_max + 1``` in the code.
After adding the description, add ```#define NR_syscalls 334``` by the way, it is ```__NR_syscall_max + 2``` in the code.
This line of description allows system call number to provide upper and lower bounds.


6. Define the function prototype of the system call
Path: ```/usr/src/linux-headers-5.11.0-27-generic/arch/x86/include/asm/syscalls.h```.  
Next, she added function prototype of ```asmlinkage int sys_mycall(void);```  
```cd /usr/src/linux-headers-5.11.0-27-generic/arch/x86/include/asm/```  
Next, ```nano syscalls.h```  
Afterwards, add ```asmlinkage int sys_mycall(void);```  

7. Add the main code of system call  
Path: ```/usr/src/linux-headers-5.11.0-27-generic/kernel/```  
```cd /usr/src/linux-headers-5.11.0-27-generic/kernel/```  
The purpose here is to write the code for the new system call. Note that the name of the newly added code file must be the same as the name of the system call.  
For example, here you have to add a file named ```mycall.c```. Otherwise, the os can't find the correct system call correspondence.  
```nano mycall.c```

8. Makefile configuration  
Path:```/usr/src/linux-headers-5.11.0-27-generic/kernel/Makefile```  
```cd /usr/src/linux-headers-5.11.0-27-generic/kernel/```  

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Add your own new file to be compiled after ```obj-y```.  The variable ```obj-y``` represents the set of target file names that need to be compiled into the kernel, and the following example adds ```mycall.o```.  
&nbsp;&nbsp;&nbsp;&nbsp;To finish the action, do the following action:  ```nano Makefile```
    
## Compile
Path: ```/usr/src/linux-headers-5.11.0-27-generic/```  
```cd /usr/src/linux-headers-5.11.0-27-generic/```  
1. Delete intermediate files  
```make mrproper```  
```make clean```  

2. By using menuconfig  
My computer is missing the components:  
```sudo apt-get install flex```  
```apt-get install bison -y```  
Start up command: ```make menuconfig```  
Get stuck. Afterwards, I did it on my own under Linux kernel-5.8.1. And there are serval probelms we encountered. They are described below.
--------
Discussed issues are below:
&nbsp&nbsp;&nbsp;&nbsp;What is ```asmlinkage```? When the system call handler wants to call the corresponding system call routine, it pushes the value of the general-purpose scratchpad onto the stack. Therefore, the system call routine has to use the stack to read the parameters passed by the system call handler. Then, that's what the ```asmlinkage``` tag is for. After adding ```asmlinkage```, C function will take parameters from stack instead of taking parameters from register.  
```#define asmlinkage CPP_ASMLINKAGE_attribute_((regparm(0)))```  
- regparm(0) means not to pass parameters from scratchpad.
