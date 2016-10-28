TARGET=print_pid 
	#the name of target module

KDIR=/usr/src/linux-headers-$(shell uname -r) 
	#the path of kernel

PWD=$(shell pwd) 
	#the current path of this module

obj-m:=print_pid.o 
	#the .o file to be produced

default:
	make -C $(KDIR) M=$(PWD) modules

