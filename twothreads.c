#include <stdio.h>
#include <stdlib.h>
#include <cyg/io/flash.h>
#include <cyg/kernel/kapi.h>

#define ADDR  0x9f280000
#define SECTOR_SIZE 4096



int mem_cpy ( void *dest, const void *addr, unsigned count)
{
		while (count-- > 0) {                                                        

				*((unsigned char *)dest) = *((unsigned char *)addr);                               

				addr ++;                                                            

				dest ++;                                                            
		} 
}

int setconf(void *buf, size_t size)
{
		
		int ret ;

        cyg_flashaddr_t err_address;

		if (  ( ret = cyg_flash_init(NULL) ) ) {

				return ret ;
		}

		char *ram_base = malloc ( SECTOR_SIZE );

		if ( NULL == ram_base ) {

				diag_printf ("memallocation failed\n");

				return -1; 

		}

		memset(ram_base, '\0', SECTOR_SIZE);

		mem_cpy(ram_base, buf, size);

        if (  ( ret = cyg_flash_erase(ADDR, 1, &err_address) ) ) {

            return (ret) ;

        }

        if ( ( ret = cyg_flash_program(ADDR, ram_base, SECTOR_SIZE, &err_address))) {

            return (ret) ;

        }

		return size;
}
void getconf(void *buf)
{
		if (  ( ret = cyg_flash_init(NULL) ) ) {

				return (ret) ;

		}

		mem_cpy(buf, ADDR, SECTOR_SIZE);

		return;
}
void cyg_user_start(void)
{
}

