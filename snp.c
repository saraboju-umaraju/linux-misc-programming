#include <stdio.h>

int main (int argc, char *argv[])
{
	char buf[4] = {};
	 snprintf(buf, sizeof(buf), "%d", 2);
	printf("%s\n", buf);
	while(1);
	return 0;
}

