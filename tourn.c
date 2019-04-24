#include <stdio.h>

#define MAX 7

int bigger (int a, int b)
{
	int k = 0;
	(a>b)?(k = a):(k = b);
	return k;
}
int main (int argc, char *argv[])
{
	long *ptr = malloc( (sizeof (long*) * ((2*MAX))));
	/* validation pending */

	int l = 0;
	int k = 0;
	for (l = MAX ;l<=(2*MAX)-1 ; l++)	
		ptr[l] = l+99; 
	
	for(l =(2*MAX)-1, k=MAX-1; l >= 0; l-=2,k--){
		ptr[k] = bigger(ptr[l], ptr[l-1]);
	}	
	
	for (l = 1 ;l<=(2*MAX)-1 ; l++)	
		printf ("%d ",ptr[l]); ; 

	printf ("\n");
	return 0;
}

