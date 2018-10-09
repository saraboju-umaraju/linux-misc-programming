#include <stdio.h>
#include <math.h>

int main (int argc, char *argv[])
{
	double answer ;
	double angle  = 90 ;
	asm volatile ("fsin" : "=t" (answer) : "0" (angle));
	printf("%lf\n", answer);
	return 0;
}

