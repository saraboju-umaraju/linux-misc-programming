#include <stdio.h>

#define swap_char(str, a, b) \
		str[a] = (str[a] ^ str[b]) ;\
		str[b] = (str[a] ^ str[b]) ;\
		str[a] = (str[a] ^ str[b]) ;

char *tmp = NULL ;
char *rev_str(char *str)
{
	int len = strlen(str);
	int len1 = len;
	int index = 0;	
	while( index < (len1/2) ) {
		swap_char(str, index, len-1);	
		index++;
		len--;
	}
	return str;
}
char* fromdeci(int num, int base)
{
	tmp = malloc(100);

	int index=0;
	while(num) {
		if ((num % base) > 9) {
			printf("%c ", (55+(num % base)));
			tmp[index++] = (55+(num % base));
		} else {
			printf("%d ", ((num % base)));
			tmp[index++] = ( '0' +(num % base));
		}
		num/=base;
	}
	tmp[index++] = 0;
	
	printf("tmp %s\n",tmp);
	
	return rev_str(tmp);
		
}

int value(unsigned char ch)
{
	printf("got %d\n", ch);
	printf("made %d\n", ch);
	if (ch >= '0' && ch <= '9')
			return (int)ch - '0' ;
	ch = ch & (~(1<<5));
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
		printf("%d %d\n", val, final);
		power*=base;
		printf(" power %d\n", power);
	}
	printf("dec %lu\n", final);
}
#define mkstr(x) mkstr_1(x)
#define mkstr_1(x) #x

int validate(char *str, int base)
{
	int index = 0;

	int max = 0;

	int tmp = 0;

	if ( base < 2 || base > 36)
			return 1;
	
	for(; str[index] ; index++) {

		max = ( ( max < str[index] ) ? str[index] : max );
	
	}

	if ((max >= 'a' && max <='z') || (max >= 'A' && max <='Z') ) {
		max = max & (~(1<<5));
		tmp = ((int)max - 'A' + 10);
	}
	if ((max >= '0' && max <='9')) {
		tmp = (int)max - '0';
	}
	

	if (tmp > 36 || base <= tmp )
		return 1;
	
	return 0;
		
}

int main (int argc, char *argv[])
{
	//fromdeci(_val("number"), 18); //decimal number, and to which base to convert
	//char str[] = "DFDF";

	char str[] = mkstr(NUMBER);
	printf ("str %s\n", str);
	if (validate(str,BASE)) {
		printf ("wrong combination\n");
		return -1;
	}
	todec(str,BASE); // any base number and the base

	//todec(fromdeci(NUMBER, BASE), BASE);
	return 0;
}

