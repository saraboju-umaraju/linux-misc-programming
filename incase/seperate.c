#include "decs.h"

#if 0
Algorithm:
…1) Get pointer to the last node.
…2) Move all the odd nodes to the end.
……..a) Consider all odd nodes before the first even node and move them to end.
……..b) Change the head pointer to point to the first even node.
……..b) Consider all odd nodes after the first even node and move them to the end.
#endif

#define insert_end(x,y) do {\
						x->next = y;\
						x = y;\
						}while(0);
#define Node list
#define head_ref head_t
#define data guy
int seperate_even_odd(list **head_t) 
{
#if 1
	list *head = *head_t;
	list *endptr = *head_t;
	list *tmp = NULL;
	list *parent = NULL ;
	list *endptrp = NULL ;

	 while (endptr->next != NULL)  
        endptr = endptr->next;
	
	endptrp = endptr;
	
	while( (head->guy %2 != 0) && head != endptr) {
			endptr->next = head;
			head = head->next;
			endptr->next->next = NULL;
			endptr = endptr->next;
	}

	if ( head->guy %2 == 0) {
		*head_t = head;
		while(head != endptrp) {
			if ( head->guy %2 == 0) {
				parent = head;
				head = head->next;
			}else {
				parent->next = head->next;
				head->next = NULL;
				endptr->next = head;
				endptr = head;
				head = parent->next;
			}
		}

	}	
#else
	Node *end = *head_ref;  
    Node *prev = NULL;  
    Node *curr = *head_ref;  
  
    while (end->next != NULL)  
        end = end->next;  
  
    Node *new_end = end;  
  
    while (curr->data % 2 != 0 && curr != end)  
    {  
        new_end->next = curr;  
        curr = curr->next;  
        new_end->next->next = NULL;  
        new_end = new_end->next;  
    }  
  
    if (curr->data%2 == 0)  
    {  
        /* Change the head pointer to  
        point to first even node */
        *head_ref = curr;  
  
        /* now current points to 
        the first even node */
        while (curr != end)  
        {  
            if ( (curr->data) % 2 == 0 )  
            {  
                prev = curr;  
                curr = curr->next;  
            }  
            else
            {  
                /* break the link between 
                prev and current */
                prev->next = curr->next;  
  
                /* Make next of curr as NULL */
                curr->next = NULL;  
  
                /* Move curr to end */
                new_end->next = curr;  
  
                /* make curr as new end of list */
                new_end = curr;  
  
                /* Update current pointer to 
                next of the moved node */
                curr = prev->next;  
            }  
        }  
    }
#endif

	return 0;

}
