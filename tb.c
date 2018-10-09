#include <stdio.h>

#define ath_reg_rd(_phys)       (*(volatile ath_reg_t *)KSEG1ADDR(_phys))        
                                                                                 
#define ath_reg_wr_nf(_phys, _val) \                                             
        ((*(volatile ath_reg_t *)KSEG1ADDR(_phys)) = (_val))                     
                                                                                 
#define ath_reg_wr(_phys, _val) do {    \                                        
        ath_reg_wr_nf(_phys, _val);     \                                        
        ath_reg_rd(_phys);              \                                        
} while(0)                                                                       
            
#define KSEGX(a)                (((unsigned long)(a)) & 0xe0000000)              
#define KSEG0ADDR(a)            ((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | KSEG0))
#define KSEG1ADDR(a)            ((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | KSEG1))
#define KSEG2ADDR(a)            ((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | KSEG2))
#define KSEG3ADDR(a)            ((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | KSEG3))
int main (int argc, char *argv[])
{
	ath_reg_wr(ath_reg_wr(0x18060008) & 0xffffff00 );
	return 0;
}

