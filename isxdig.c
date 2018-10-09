#include <stdio.h>

int main (int argc, char *argv[])
{
	unsigned char c = 0xA;
	printf ("%x", isxdigit(c));
	return 0;
}

