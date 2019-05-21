
#include "decs.h"

int remdup (list *head)
{
	list *tmp = head;
	
	if ( NULL == head ) {
			return -1;
	}

	for(; head ; head = head ->next)	
		printf ("%d ", head->guy);
	head = tmp ;
	while (head->next) {
			if (head->next->guy == head->guy) {
				tmp = head->next;
				head->next = head->next->next;
				free(tmp);
				tmp = NULL ;
			}else{
				head = head->next ;
			}
	}

	return 0;
}

