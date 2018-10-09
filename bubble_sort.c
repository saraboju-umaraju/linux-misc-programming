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

void bubsort(int *arr, int len)
{
	int l = 0;
	int k = 0;
	
	for(;l<len-1; l++)
		for(k=l+1; k<len; k++) 
			if (arr[l] < arr[k]) 
					_swap(arr[l],arr[k]);
	
}

int main (int argc, char *argv[])
{

	int arr[] = {4,2,6,8,2,1,7,9,-3,-34,1,6,4,3};
	bubsort(arr,ARRAY_SIZE(arr));
	print_arr(arr,ARRAY_SIZE(arr));
	puts("");
	return 0;
}

