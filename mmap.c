#include <stdio.h>
#include <sys/mman.h>

int main (int argc, char *argv[])
{

	FILE *fp = fopen(argv[0], "r");

	FILE *fp1 = popen("ls", "r");

	int new = dup(fileno(fp1));
	
	if (!fp1)
		return -1;

	printf ("%x\n", fp1);
	(mmap(0,4096, PROT_READ, MAP_ANON,fileno(fp1),0));
	perror("");
	return 0;
}

