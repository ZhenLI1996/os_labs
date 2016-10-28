TARGET=print_pid #the target name
KDIR=/usr/src/linux-headers-4.8.0-22-generic #the path where the linux source code can be found
PWD=$(shell pwd) #the current path where the module file can be found
obj-m:=print_pid.o #the object file to be produced
default:
	make -C $(KDIR) M=$(PWD) modules

