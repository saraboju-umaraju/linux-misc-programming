
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	char dst[100];

	fd = open("/sys/spiDev/spidev", O_RDWR);

	if (fd < 0)
		perror("Unable to open the device*** \n");
	else{
		printf("File opened Sucessfully %d\n", fd);

//		write (fd, dst, 30 );

//		read (fd, dst, 30);

		close(fd);
	}
	return 0;
}

