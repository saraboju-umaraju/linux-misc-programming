/* C implementation QuickSort */
#include<stdio.h>

void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

int partition (int arr[], int min, int max)
{
    int i;
    int j;
    int pivot;
    pivot = arr[min];
    i = min+1;
    j = max;

    while(1){

        while(arr[i] < pivot)
            i++;
        while(arr[j] > pivot)
            j--;

        if(i < j)

            swap(&arr[i], &arr[j]);
        
        else
            
            break;
        }


    arr[min] = arr[j];

    arr[j] = pivot;
  //  swap_arr(&arr[min], &arr[j]);

   return j;

}

void quickSort(int arr[], int low, int high)
{
	if (low < high)
	{
		int pi = partition(arr, low, high);

		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

/* Function to print an array */
void printArray(int arr[], int size)
{
	int i;
	for (i=0; i < size; i++)
		printf("   %d ", arr[i]);
	printf("n");
}

// Driver program to test above functions
int main()
{
	int arr[] = {10, 7, 8, 9, 1, 11,112,6,0};
	int n = sizeof(arr)/sizeof(arr[0]);
	quickSort(arr, 0, n-1);
	printf("Sorted array: n");
	printArray(arr, n);
	return 0;
}

