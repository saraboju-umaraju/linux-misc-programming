#Makefile for a kernel module

obj-m += i82540net.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	dmesg -C

i:
	insmod i82540net.ko

c:
	rmmod i82540net
