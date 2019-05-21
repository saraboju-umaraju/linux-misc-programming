#include <stdio.h>

int movedup(int a[], int n)
{
	int i,j,k,l;

	for(i=0;i<n;++i)
		for(j=i+1;j<n;)
		{
			if(a[i]==a[j])
			{
				for(k=j;k<n-1;++k)
					a[k]=a[k+1];

				--n;
			}
			else
				++j;
		}
#if 0
	for(i=0; i<n; i++) {
		for(j=i+1; j<n;) {
			if (arr[i] == arr[j]) {
				for(k=j;k<n-1;k++) 
					arr[k] = arr[k+1];
				
				n--;
			} ekse {
				j++;
			}
		}
	}	

#endif

	return 0;
}

int main (int argc, char *argv[])
{
	//int arr[] = {1,2,3,1,3,4,1,3,4,5,2,5,3,56,5,3};
	int arr[] = {1,1,2,2,3,4};
	movedup(arr, sizeof (arr)/sizeof (arr[0]) );
	int size = sizeof (arr)/sizeof (arr[0]);
	int index = 0;

	for(; index<size; index++)
		printf ("%d ", arr[index]);
	return 0;
}

