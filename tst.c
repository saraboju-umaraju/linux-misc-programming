#include <stdio.h>

int main (int argc, char *argv[], char *envp[])
{
//	__environ;
	#if 0
	asm("mov $0x66, %rdi\n\t"
    "call putchar\n\t");
	int a,b ;
	printf ("%d",a++-++b);
	return 0;
	#endif
	printf ("%u\n", sizeof argv[1]);
//	typedef unsigned int @ufile_ptr;

}

