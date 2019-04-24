#include <stdio.h>

int main (int argc, char *argv[])
{
	int a [2] [3] = {{3,2,3},
		{1,2,3}};

	int b [3] [2] = {{1,1},
		{2,2},
		{3,4}};

	int i=0;
	int j=0;
	int k=0;
	int sum=0 ;

#define A_ROW 2 //m r1
#define A_COL 3 //n c1

#define B_ROW 3 //p r2
#define B_COL 2 //q c2

	/*loop row num of times in A */
	for(i=0; i<A_ROW; i++)
		/*loop column num times in B */ 
		for(j=0; j<B_COL; j++) {
			/* loop A_COL or B_ROW times */
			for(k=0; k< B_ROW; k++){
				sum += a[i][k] * b[k][j];
				printf ("%d %d %d %d\n", i,k,k,j);
			}
			printf ("%d \n", sum);
			puts("");
			sum = 0 ;
		}

	return 0;
}

