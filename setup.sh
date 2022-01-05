mkdir get_number_of_entering_wait_queue 
vim get_number_of_entering_a_wait_queue/get_number_of_entering_a_wait_queue.c
vim get_number_of_entering_a_wait_queue/Makefile
vim Makefile
vim include/linux/syscalls.h
vim arch/x86/entry/syscalls/syscall_64.tbl

make menuconfig
make -j8
sudo make modules_install -j8
sudo make install -j8
sudo update-grub

