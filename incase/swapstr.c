#include <stdio.h>

void swapptrs(char **o, char **m)
{
	*o = ((unsigned long)*o) ^ ((unsigned long)*m);
	*m = ((unsigned long)*o) ^ ((unsigned long)*m);
	*o = ((unsigned long)*o) ^ ((unsigned long)*m);
}

int main (int argc, char *argv[])
{
	char *str = "uma" ;
	char *str1 = "matha";
	
	swapptrs(&str, &str1);
	printf ("%s %s\n", str,str1);
	return 0;
}

