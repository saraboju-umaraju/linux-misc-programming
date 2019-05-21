#include "decs.h"

int easy_swap (list *head, int one, int two, enum by by)
{
	if (NULL == head)
		return -1;
	by = BYVAL;
	list *tmp_one = search(head, one, by);
	list *tmp_two = search(head, two, by);
	
	if ( (tmp_one == NULL && head->guy != one ) || (tmp_two == NULL && head->guy != two ))
			return 1;

	list *tone = tmp_one->next;
	list *ttwo = tmp_two->next;
	list *extra = ttwo->next;
	tmp_one->next = tmp_two->next;
	ttwo->next = tone->next;
	tmp_two->next = tone;
	tone->next = extra;
	return 0;
}

