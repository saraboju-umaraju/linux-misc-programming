#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <assert.h>

#define likely(expr) __builtin_expect(!!(expr), 1)
#define unlikely(expr) __builtin_expect(!!(expr), 0)

int fib()
{
	int arr[] = {1,3,5,7,9,11,13,14,15,17,111,19};
	srand(getpid());
	return arr[rand()%((sizeof(arr)/sizeof(arr[0]) )-1)] & 1 ;
}

int main(int argc, char **argv)
{
	int x = fib();
/*
	if (__builtin_expect(!!x,1)) {
		printf("yes returned 1");
		return 0;
	}
*/
/*
	if ( likely (x > 0)	) {
		 printf("yes returned 1");
		return 0;
	}
*/
	
	if ( unlikely (x == 0)	) {
		 printf("yes returned 0");
		return 0;
	}

    return 1;
}
