#include <stdio.h>

#define ATH_SPI_CMD_SECTOR_ERASE_4K 0x20

int erase_page(unsigned int addr)
{
	printf ("I got %x to erase\n",addr);
	//ath_spi_sector_erase(addr);
}

int main (int argc, char *argv[])
{
	unsigned int start = 0x9f009000 ;
	unsigned int size = 980 *1024  ;

	int l = 0 ;

	while ( start < (0x9f009000 + (980 *1024)) ) {

		erase_page(start);

		start+= 4096;	
	}

	return 0;
}

