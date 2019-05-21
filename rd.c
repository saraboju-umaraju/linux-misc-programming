char *removeDuplicate(char str[], int n) 
{
	char *tmp = str ;
	
	int index = 1 ;

	while(*tmp) {

		printf ("%c\n", *tmp);

		index = 1;
		while (*(tmp+index)) {
			if ( *tmp == *(tmp+index) ) {
				
				printf ("\t%c\n", *tmp);
				memmove(tmp, tmp+index, n-index);
				n--;
			}
			index++;
		}	
		tmp++;
	}

	return str;
} 

// Driver code 
int main() 
{ 
	char str[]= "umumumumarajurajuumaraju"; 
	int n = sizeof(str) / sizeof(str[0]); 
	printf (removeDuplicate(str, n));
	return 0; 
} 

