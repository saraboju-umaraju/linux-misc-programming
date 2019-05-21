#include "decs.h"

int check_failure(void)
{
	return 0;
}

int alloc_item ( list **client, int data)
{
	*client = malloc (sizeof (list) );
	if (*client == NULL ) {
		status_g = 1 ;
		return 1;
	}
	list *client_t = *client;
	client_t->guy = data ;
	client_t->next = NULL;
	return 0;
}

