KERNEL_BUILD := /lib/modules/`uname -r`/build

obj-m := oops.o

modules:
	@$(MAKE) -C $(KERNEL_BUILD) M=$(PWD) $@
clean:
	@$(MAKE) -C $(KERNEL_BUILD) M=$(PWD) $@

