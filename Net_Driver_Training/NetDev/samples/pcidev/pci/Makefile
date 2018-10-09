KERNEL_BUILD := /lib/modules/`uname -r`/build
obj-m := pcires.o
modules:
	@$(MAKE) -C $(KERNEL_BUILD) M=$(PWD) $@
clean:
	@$(MAKE) -C $(KERNEL_BUILD) M=$(PWD) $@
