#include <stdio.h>

static unsigned long long rdtsc(void)
{
	unsigned long long val, low, high;

	#define EAX_EDX_RET(val, low, high)	"=A" (val)

	#define EAX_EDX_VAL(val, low, high)	(val)

	asm volatile("rdtsc" : EAX_EDX_RET(val, low, high));

	return EAX_EDX_VAL(val, low, high);
}
int main (int argc, char *argv[])
{
	unsigned long long x = rdtsc();

	printf ("%llu \n", x);

	return 0;
}

