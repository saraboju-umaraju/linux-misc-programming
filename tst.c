#include <stdio.h>

void alloc(char **ptr)
{
	*ptr = malloc(16);
}
int main (int argc, char *argv[])
{
	char *ptr = NULL;
	alloc(&ptr);
	printf("%x\n", ptr);
	return 0;
}

