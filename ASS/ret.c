#include <stdio.h>

extern long lol();
int main (int argc, char *argv[])
{
	printf("%d\n", lol());
	return 0;
}

