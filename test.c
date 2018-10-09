#include <stdio.h>
#define ath_be_msb(_val, _i) (((_val) & (1 << (7 - _i))) >> (7 - _i))

#define ath_spi_bit_banger(_byte)   do {                \

int
main(int argc, char *argv[])
{
    int i;
    for (i = 0; i < 8; i++) {
        printf("%x\n", ath_be_msb(0x0a, i));
    }
    return 0;
}
