obj-m := vram.o

modules:
	make -C /usr/src/linux-4.13.2 M=`pwd` $@
clean:
	make -C /usr/src/linux-4.13.2 M=`pwd` $@
