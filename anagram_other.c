#include <stdio.h>
#include <types.h>
#define LIM 100


bool
is_anagram(char *str)
{
	return true;
}


int main (int argc, char *argv[])
{
	char lim_buffer [LIM] = {};
	int status = -1 ;

	printf ("gimme a string to work with\n");
	status = fgets(lim_buffer, LIM-1, stdin);
	if (status == NULL) {
		fprintf(stderr, "input failed\n");
		return status;
	}
	
	is_anagram(lim_buffer);
	return 0;
}

