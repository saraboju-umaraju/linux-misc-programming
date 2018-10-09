#include <stdio.h>

extern char *str __attribute((__weak__)) = "hello";

int sum (int argc, char *argv[])
{
	puts(str);
	return 0;
}

