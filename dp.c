#include <stdio.h>

int main (int argc, char *argv[])
{
	char ** hello = NULL ;
	int k = 0 ;
	
	hello = (char **)malloc (10 * sizeof (void*));

	for( ; k<10; k++) {
		hello[k] = malloc (10);
		strncpy(hello[k], "crap", 5);
	}
	
	for( k=0 ; k<10; k++) {
		printf ("%s\n", hello[k]);
	}
	return 0;
}

