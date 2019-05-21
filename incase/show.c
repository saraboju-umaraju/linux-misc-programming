#include "decs.h"

void show (list **h)
{
	list *head = *h;
	for(; head ; head = head->next) {
		printf ("%d ", head->guy);
	}
	printf ("\n");
	return ;
}

