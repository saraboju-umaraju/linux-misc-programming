obj-m:= for_each_dev.o

#obj-m:= pci-driver.o

#obj-m:= emulate.o

#obj-m:= copy_PCI.o

target:
	make -C /lib/modules/3.13.0-32-generic/build/ M=`pwd` modules
clean:
	make -C /lib/modules/3.13.0-32-generic/build/ M=`pwd` clean
