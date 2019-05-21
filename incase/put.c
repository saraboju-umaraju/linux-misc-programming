#include "decs.h"

void link_next(list *p, list *link)
{
	p->next = link;
}

list *find_apt_pos(list **head_t, int member)
{
	list *poten = *head_t ;	
	list *head = *head_t ;

	if (member < head->guy) 
		return NULL ;

	for(; head ; poten = head, head = head->next) {

		if (NULL == head || (member < head->guy)) {
			printf ("### %d ###\n", poten->guy);
			return poten ;
		}

	}

	return poten ;
}

int put (list **head_t, int member)
{
	list *head = *head_t ;
	list *new = NULL ;
	list *to_after = NULL ;

	if ( alloc_item( &new,member ) ) {
		return 1;
	}

	if (head == NULL) {
		*head_t = new ;
	}else {
#ifndef UNSORT
		link_next(new, *head_t);
		*head_t = new;
#else
		to_after = find_apt_pos(head_t, member);
		if (to_after == NULL){
			link_next(new, *head_t);
				*head_t = new ;
		} else {
			link_next(new, to_after->next);
			link_next(to_after, new);
		}
#endif
	}
	return 0;
}

