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

***Explanation***: In the file ```unistd.h```, add the number of system call, 
that is to add ```#define __NR_mycall```, and it is defined as the 333rd system call number, it is ```__NR_syscall_max + 1``` in the code.
After adding the description, add ```#define NR_syscalls 334``` by the way, it is ```__NR_syscall_max + 2``` in the code.
這一行敘述，讓system call number提供上下界


6、定义system call的函式原型
路徑位置：/usr/src/linux-headers-5.11.0-27-generic/arch/x86/include/asm/syscalls.h
新增asmlinkage int sys_mycall(void);的函式原型
cd /usr/src/linux-headers-5.11.0-27-generic/arch/x86/include/asm/
nano syscalls.h
    asmlinkage int sys_mycall(void);

7、新增system call的主程式碼
路徑位置：/usr/src/linux-headers-5.11.0-27-generic/kernel/
cd /usr/src/linux-headers-5.11.0-27-generic/kernel/
這邊的目的就是撰寫新增的system call的程式碼，
注意新增的程式碼檔名得要跟system call的名稱一樣，
像在這邊就得要新增一個名為mycall.c的檔案，
否則則系統無法找到正確的 system call 對應
nano mycall.c

8、Makefile的設定
路徑位置：/usr/src/linux-headers-5.11.0-27-generic/kernel/Makefile
cd /usr/src/linux-headers-5.11.0-27-generic/kernel/
在obj-y後面新增自己新增要編譯的檔， 變數 obj-y 表示需要編繹到內核中的目標檔案名集合，如下範例新增mycall.o
nano Makefile
    
## 编译文件
位置：/usr/src/linux-headers-5.11.0-27-generic/
cd /usr/src/linux-headers-5.11.0-27-generic/
1、删除中间档案
make mrproper
make clean

2、通过使用menuconfig
我的电脑缺少组件：
sudo apt-get install flex
apt-get install bison -y
启动： make menuconfig
卡住了~
