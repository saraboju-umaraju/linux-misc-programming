#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fs.h>
char *file_name = "/dev/sda1";

int main (int argc, char *argv[])
{
	struct stat dev;
	unsigned long long int size ;
	int fd = open (file_name, O_RDONLY);
	if (fd == -1)
		return fd;
	ioctl(fd, BLKGETSIZE64, &size);

	printf ("%llu\n", size);
	return 0;
}

