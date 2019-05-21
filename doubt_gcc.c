#include <stdio.h>

int main (int argc, char *argv[])
{
	long long int block = 31997250 * 4096L; //_val(argv[1]);
    long long int block_1 = 131060736000; //

	if ( block == block_1) 
		printf ("equal\n");
	else 
		printf ("not equal\n");
	return 0;
}

