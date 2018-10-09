#include <stdio.h>

void towers(int n, int from, int to, int via)
{
	if (n == 1) { 
		printf (" %c to %c \n",from, to);
		return;
	}
	towers (n-1,from,to,via);
	printf (" %c to %c \n",from, to);
	towers (n-1,via,to,from);
}

int main(void)
{
	int n = 5;
	towers (n,'a','b','c');
	return 0;
}
