
#include "decs.h"

int reverse (list **head_t)
{
	list *head = *head_t;
	list *prev = NULL;
	list *next = NULL;
	list *current = head ;

	if ( NULL == head ) {
			return -1;
	}

	while (current) {
			next = current->next;
			current->next = prev ;
			prev = current ;
			current = next;
	}

	*head_t = prev ;

	return 0;
}

