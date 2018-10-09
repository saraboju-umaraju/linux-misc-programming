#include <stdio.h>

int main (int argc, char *argv[])
{
	int l = 0;
	
	for(;l<500;l++) {
		if (0 == (l & 0x0f)) {
			printf("%d\n",l);
		}
	}
	return 0;
}
