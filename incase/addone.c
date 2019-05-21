
#include "decs.h"

int addone (list **head_t)
{
	reverse(head_t);

	list *head = *head_t;

	while (head) {
		if (( (head->guy) + 1) <= 9 ) {
			head->guy +=1;
			reverse(head_t);
			return 0;
		}else {
			head->guy = 0;
			head = head->next ;
		}
	}

	put(head_t, 1);
	reverse(head_t);

	return 0;
}

int delete_last(list **h, int data)
{
	list *head = *h;
	list *tmp = NULL ;
	list *parent = NULL ;
	list *p1 = NULL ;
	for(; head ; ) {
		if (head->guy == data) {
			p1 = parent;
			tmp = head;
		}
		parent = head;
		head = head->next;
	}

	if (tmp == (*h) ) {
		*h = (*h)->next;
		free(tmp);
		tmp = NULL;
	}else if (NULL != tmp) {
		if (NULL == tmp->next) {
			free(tmp);
			tmp = NULL;
			printf ("%d\n", p1->guy);
			p1->next = NULL ;
		}else {	
			tmp->guy = tmp->next->guy;
			parent = tmp->next;
			tmp->next = tmp->next->next;
			free(parent);
			parent = NULL ;
		}
	}

}
