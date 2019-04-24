#include <stdio.h>

double somefunc_1(char (*ptr)(int))
{
	char ch = (*ptr)(10);
	
	return 1.0 ;
}

double (*somefunc_1_p)(char (*)(int)) = &somefunc_1;

void somefunc(float data)
{
	return ;
}

void (*(        someotherfunc(int data)     ))(float)
{
	return &somefunc ;
}

int main (int argc, char *argv[])
{

#if 0
	int uma = 12 ;

	/* pointer to integer */
	
	/* uma == (*ptr) */

	int (*ptr) = &uma ;

	printf ("%d %d\n", uma ,(*ptr));

	int umarray [12] = {} ;

	/* umarray[] == (*umarray_p)[] */

	int (*umarray_p)[] = &umarray ;

	int umarray_2d [12][13] = {} ;

	int (*umarray_2dp) [12][13] = &umarray_2d ;

	int (*mainp) (int argc, char *argv[]) = &main ;

	int (*(*mainp_p)) (int argc, char *argv[]) = &mainp;

	int (*(*(*mainp_p_p))) (int argc, char *argv[]) = &mainp_p;

	/* pointer to array of function pointers */

	void (*func_ptr)(float) ;

	void (*func_ptr[])(float);

#endif

	int list[4][5] = { {1,2,3,4,5},
					   {11,12,13,14,15},
					   {21,22,23,24,25},
					   {31,32,33,34,35}
					  };

	int (*p)[5] = list;

	printf ("%d\n", p[0][4]);
	printf ("%d\n", list[0][4]);

	int three_d[2][3][4] = 
		{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24} ;
		/* { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16}, {17,18,19,20}, {21,22,23,24} }*/
	
	return 0;
}
