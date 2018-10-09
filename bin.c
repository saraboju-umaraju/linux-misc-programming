#include <stdio.h>
#include <string.h>


unsigned int convert( char *arg)
{
	int l = strlen (arg) - 1 ;
	int k = 0;
	unsigned int byte = 0;
	for(; l >= 0 ; l--,k++) {
		byte |= (((arg[l])-'0') << k);	
	}
			
	return byte;
}

char *num_to_bits( unsigned char number)
{
	int l = 0;
	static char str[8] ;
	for(; l < 8; l++) {
		 str[7-l] = (((number>>l) & (1)) ) + '0';
	}
	str[8] = 0;
	return str;
}

int main (int argc, char *argv[])
{
	unsigned char byte = 0;
	byte = convert ( num_to_bits(11));
	return byte;
}

