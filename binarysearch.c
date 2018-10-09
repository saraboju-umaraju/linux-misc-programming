#include <stdio.h>

int get_index(int *arr, int low, int high, int key)
{
	if ( low > high) 
		return -1;
	int mid = (low+high)/2;
	if (arr[mid] == key) 
		return mid;
	else if (arr[mid] > key) 
		return get_index(arr,low,mid-1,key);
	else return get_index(arr,mid+1,high,key);
}

int main(void)
{
	int arr[] = { 1, 4, 6, 9, 13, 15, 24, 27, 37, 56, 67, 78};
	printf("%d \n",get_index(arr,0,sizeof(arr)/sizeof(arr[2]),25));
	return 0;

}
