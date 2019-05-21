
#include "decs.h"

int findNth (list **head_t, int n)
{
	list *head = *head_t;
	list *tmp = *head_t;

	int tmpn = n ;

	if (NULL == head) 
			return -1;
	
	do{
			head = head->next;
	}while(n-- && head);

	if (NULL == head) 
			return -1;
	
	while(head) {
		head = head->next;
		tmp = tmp->next;
	}
	
	printf (" %d th node is %d\n", tmpn, tmp->guy);
	
	return 0;
}

int palutil(list **head, list *two)
{
	if ( NULL == (two)  ) {

		return 1;
	}
	int isp = palutil((head), two->next);

	if (!isp) 
		return 0;

	int isp1 = (two->guy == (*head)->guy) ;

	printf("%d %d\n",(*head)->guy, two->guy);

	*head = (*head)->next ;

	return isp1 ;
}

int palindrome(list *head_t) 
{
	printf ("%d\n", palutil(&head_t, head_t));

}
