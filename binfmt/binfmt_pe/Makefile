obj-m   := src/binfmt_pe.o

KDIR    := /lib/modules/$(shell uname -r)/build
PWD     := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm $(PWD)/modules.order $(PWD)/Module.symvers
	$(MAKE) -C $(KDIR) M=$(PWD) clean          
