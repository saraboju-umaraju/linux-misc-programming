#include <stdio.h>

void fromdeci(int num, int base)
{
	while(num) {
		if ((num % base) > 9) 
			printf("%c ", (55+(num % base)));
		else
			printf("%d ", ((num % base)));
		num/=base;
	}
		
}

int value(char ch)
{
	//printf("got %d\n", ch);
	if (ch >= '0' && ch <= '9')
			return (int)ch - '0' ;
		return (int)ch - 'A' + 10;

}
void todec(char *str, int base)
{
	int power = 1;
	int len = strlen(str);
	int index=len-1;
	int val = 0;
	int final = 0;
	
	for(; index >= 0; index--) {
		val = value(str[index]);
		final += (val*power) ;
		//printf("%d %d\n", val, final);
		power*=base;
		//printf("%d\n", power);
	}
	printf("%lu\n", final);
}

int main (int argc, char *argv[])
{
	fromdeci(_val("number"), 18); //decimal number, and to which base to convert
	char str[] = "DEADGH";
	todec(str,18); // any base number and the base
	return 0;
}

