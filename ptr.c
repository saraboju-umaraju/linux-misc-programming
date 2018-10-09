#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	long *ptr = malloc(10*sizeof(long));
	int l=0;
	for(;l<10;l++)
		ptr[l] = l;
//	for(l=0;l<10;l++)
//		printf ("%ld ",ptr[l] );

	long **dptr = malloc(10*sizeof(long*));
		 for(l=0;l<10;l++)
			dptr[l] = malloc(sizeof(long)*10);
	int m = 0;
	
	for(l=0;l<10;l++)
		for(m=0;m<10;m++) 
			dptr[l][m] = l*m;
	
	for(l=0;l<10;l++)
		for(m=0;m<10;m++) 
			printf ("%ld ",dptr[l][m]);
	
}
