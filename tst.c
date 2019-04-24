#include <stdio.h>

int main (int argc, char *argv[])
{
	char buf[100];
	memset(buf, 0, sizeof buf);
	puts("enter number");
	fgets(buf,sizeof(buf),stdin);
	int base = _val("enter base");
	if (base > 36) {
		printf("Invalid base %d\n", base);
		exit(1);
	}

	
	return 0;
}

