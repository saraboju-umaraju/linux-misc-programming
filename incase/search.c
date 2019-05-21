#include "decs.h"

list* search (list *head, int member, enum by by)
{
	list *tmp = NULL ;

	for(; head ; tmp = head, head = head->next) {
		if (by == BYVAL){
			if ( head->guy == member )
				return tmp;
		}
	}
	return NULL;
}

