#include <stdio.h>



int main (int argc, char *argv[])
{
	int x = 12 ;
	int X = 12 ;
	int u = 12 ;
	int s = 12 ;
	int t = 12 ;
	int w = 12 ;
	int y = 12 ;
	int z = 12 ;
	int l = 12 ;
	int *ptr1= &x;
	int *ptr2 = NULL ;
	long tst = 12;
	ptr2 = &tst;
	printf("%d", ptr1-ptr2);
	return 0;
}

