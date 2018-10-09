#include <stdio.h>

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(*arr))

#define _swap(a,b) \
	do {   \
	a^=b;  \
	b^=a;  \
	a^=b;  \
	} while(0);

void print_arr(int *arr, int len)
{
	if (len) {
		print_arr(arr,len-1);
	} else {
		return;
	}

	printf("%d ",arr[len-1]);
}

void inssort(int *arr, int len)
{
	int l = 0;
	int k = 0;
	int tmp = 0 ;
	for (l=1; l<len; l++) {
			k=l;
			tmp = arr[l];
			while ( k  && tmp > arr[k-1]) {
				arr[k] = arr[k-1];
				--k;
			}
		arr[k] = tmp;
	}
}

int main (int argc, char *argv[])
{

	int arr[] = {1,4,2,6,8,2,7,9,-3,-34,1,6,4,3};
	inssort(arr,ARRAY_SIZE(arr));
	print_arr(arr,ARRAY_SIZE(arr));
	puts("");
	return 0;
}

