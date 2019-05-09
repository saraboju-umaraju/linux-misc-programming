#include <stdio.h>

#define swap_char(str, a, b) \
		str[a] = (str[a] ^ str[b]) ;\
		str[b] = (str[a] ^ str[b]) ;\
		str[a] = (str[a] ^ str[b]) ;\

char *rev_str(char *str)
{
	int len = strlen(str);
	int index = 0;	
	while( index < (len1/2) ) {
		swap_char(str, index, len-index-1);	
		index++;
	}
	return str;
}

int rev_num(int num)
{
	int rem = 0;
	int sum = 0 ;
	while(num) {
		rem = num % 10;
		num /= 10;
		sum = sum*10;
		sum += rem;
	}
	return sum ;
}

void bit_rep(int num)
{	
	int index = 0;
	
	for( ; index < sizeof (num) * 8 ; index++)
		//printf("%u", !!(num & (1<<index)));
		printf("%u", !!(num>>((sizeof (num) * 8)-index-1) & (1)));
		num>>=1;
	puts("");
}

void bit_swap(int one, int two, int pos)
{
	if ( ( one & (1 << pos) ) == ( two & (1 << pos) ) ) {
	} else {
		one = ( one ^ (1 << pos) );
		two = ( two ^ (1 << pos) );
	}
	printf ("%d %d\n", one, two);
}
int rev_num_b(int num)
{
	int bits = sizeof (num) * 8 ;
	int index =  0;
	
	for(; index < 15 ; index++) {
		if ( ( num & (1 << index) ) == ( num & (1 << (bits-index-1)) ) ) {
		} else {
			num = ( num ^ (1<<index) ^ (1 <<(bits-index-1)) ) ;
		}
	}
	return num ;
}
			
	
int main (int argc, char *argv[])
{
	int num = 1234 ;

	char uma[] = "umaraj" ;

	printf ("%d\n", rev_num(num));
	
	printf ("%s\n", rev_str(uma));

	bit_rep(10);

	bit_swap( 11, 21, 0);

	bit_rep(rev_num_b(rev_num_b(11)));

	return 0;
}
