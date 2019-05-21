
#include "decs.h"

int delete (list **head_t, int member, enum by by)
{
	list *head = *head_t;
	list *tmp = NULL;

	if (by == BYVAL) {
		if (head == NULL)
			return 1;
		list *parent = search (head, member, by);
		if (parent == NULL) {
			if (head->guy == member) {
				tmp = head ;
				head = head->next;
				*head_t = head ;
			}
		}else{
			tmp = parent->next;
			parent->next= parent->next->next ;
		}
		free(tmp) ;
		tmp = NULL ;
	}
	return 0;
}

