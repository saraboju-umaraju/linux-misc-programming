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
int b[20];
void sort(int *arr, int low,int high)
{
    int temp[100];        /*temp array to store the sorted array result*/

    int i;              /* loop variable*/
    int j;
    int k;
	int mid = (low+high)/2;

    i = low;
    j = mid + 1;

    for(k = low; (i <= mid && j <= high) ; k++){


            if(arr[i] <= arr[j])
                temp[k] = arr[i++];
            else
                temp[k] = arr[j++];
     
    }

    while(i <= mid) 
        temp[k++] = arr[i++];
    while(j <= high)
        temp[k++] = arr[j++];

    for(k = low; k<=high; k++)
        arr[k] = temp[k];
}


void mergesort(int *arr, int start, int end)
{
	int l = 0;
	int k = 0;
	if ( start >= end ) 
			return ;
	int mid = (start+end)/2;
	
	mergesort(arr,start,mid);
	mergesort(arr,mid+1,end);
	
	sort(arr,start,end);
}

int main (int argc, char *argv[])
{
	int arr[] = {1,4,2,6,8,2,7,9,-3,-34,1,6,4,3};
	mergesort(arr,0,ARRAY_SIZE(arr));
	print_arr(arr,ARRAY_SIZE(arr));
	puts("");
	return 0;
}

