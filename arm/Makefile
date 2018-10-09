target:
		arm-none-eabi-gcc -c -mcpu=cortex-a8 -g bare.c -o bare.o
		arm-none-eabi-gcc -c -mcpu=arm926ej-s -g bare.c -o fbare.o
		arm-none-eabi-as -mcpu=arm926ej-s -g startup.s -o fstartup.o
		arm-none-eabi-as -mcpu=cortex-a8 -g startup.s -o startup.o
		arm-none-eabi-ld -T target.ld fbare.o fstartup.o -o bare.elf
clean:
	rm *.o

	
