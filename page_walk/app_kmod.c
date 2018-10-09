#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>

int g_var = 10;

int main()
{
	int fd;
	void *a;

	fd = open("/dev/myChar",O_RDWR);

	if(fd < 0){
		perror("Unable to open the Device");
		exit(0);
	}

	printf ("variable value = %d\n", g_var);

	a = &g_var;

	printf ("a     = %p\n", a);

	ioctl (fd, getpid(), a);

	printf ("variable value = %d\n", g_var);
	printf ("*a value = %d\n", *(int *)a);

	close(fd);

	return 0;
}
