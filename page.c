#include <stdio.h>

#define INDEX_MASK   0xfff
#define INDEX_SHIFT  0x0

#define LEVEL1_MASK 0x1ff000
#define LEVEL1_SHIFT (12)

#define LEVEL2_MASK 0x3fe00000
#define LEVEL2_SHIFT (12+9)

#define LEVEL3_MASK 0x7fc0000000
#define LEVEL3_SHIFT (12+9+9)

#define LEVEL4_MASK 0xff8000000000
#define LEVEL4_SHIFT (12+9+9+9)

#define printUL(x) printf("%lx\n", x)

int main (int argc, char *argv[])
{
	unsigned long addr = 0xffffdeefdeadbeef ;
	
	unsigned long index = ((addr & INDEX_MASK) >> INDEX_SHIFT);

	unsigned long level_1 = ((addr & LEVEL1_MASK) >> LEVEL1_SHIFT);	

	unsigned long level_2 = ((addr & LEVEL2_MASK) >> LEVEL2_SHIFT);	

	unsigned long level_3 = ((addr & LEVEL3_MASK) >> LEVEL3_SHIFT);	

	unsigned long level_4 = ((addr & LEVEL4_MASK) >> LEVEL4_SHIFT);	

	printUL(index);	

	printUL(level_1);	

	printUL(level_2);	

	printUL(level_3);	

	printUL(level_4);	

	printUL(index | (level_4 << LEVEL4_SHIFT) | (level_3 << LEVEL3_SHIFT) | (level_2 << LEVEL2_SHIFT) | (level_1 << LEVEL1_SHIFT) );	

	return 0;
}

