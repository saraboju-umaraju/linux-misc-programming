
module:
	$(MAKE) -C /usr/src/linux-4.13.2 SUBDIRS=$(PWD) modules
 
clean:
	$(MAKE) -C /usr/src/linux-4.13.2 SUBDIRS=$(PWD) clean
 
# Otherwise KERNELRELEASE is defined; we've been invoked from the
# kernel build system and can use its language.
 
    obj-m := dor.o
    dor-y := ram_block.o ram_device.o partition.o
