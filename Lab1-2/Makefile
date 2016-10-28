kernel_path = /usr/src/linux-headers-$(shell uname -r)
PWD = $(shell pwd)
obj-m := print_pid.o
#all:
#make -C $(kernel_path)
#clean:
#make -C $(kernel_path) M=$(PWD)
make -C $(kernel_path) M=$(PWD) modules
