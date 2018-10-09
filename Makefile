obj-m:= sub.o
tar:
	make -C /lib/modules/`uname -r`/build/ M=`pwd` modules


