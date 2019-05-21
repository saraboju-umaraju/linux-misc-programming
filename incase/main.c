#include "decs.h"

int status_g = 0;

int main (int argc, char *argv[])
{
	list *head = NULL ;
	int op = -1 ; 

	int index = 10;
	if (argv[1]) {
			for (; index--; )
				put(&head, index);
	}

	while (1) {
		op = _val("Input operation\n""1.put\n2.delete\n3.showlist\n4.swap two\n5.remove dups\n6.reverse\n7.add one\n8.find nth node");
#if 0
		if (op > 4 || op < 1) {
			return 1;
		}
#endif

		switch (op) {
		case 1:
			put(&head,_val("enter data to put"));
			break;
		case 2:
			delete(&head,_val("enter data to delete"),BYVAL);
			break;
		case 3:
			show(&head);
			break;
		case 4:
			easy_swap(head,_val("swap one"),_val("swap two"),BYVAL);
			break;
		case 5:
			show(&head);
			remdup(head);
		case 6:
			reverse(&head);
			show(&head);
			break;
		case 7:
			addone(&head);
			show(&head);
			break;
		case 8:
			findNth(&head, _val("enter N"));
			break;
		case 9:
			palindrome(head);
			break;
		case 10:
			delete_last(&head, _val("delete"));
			break;
		case 11:
			seperate_even_odd(&head);
			break;
			
		default:
			exit(1);
		}
	
		__fpurge(stdin);        
	}
	
	/* allocate memory for head */
	
	if ( alloc_item( &head,1 ) ) {
		return 1;
	}

	if (check_failure()){
		exit(status_g);
	}
	return 0;
}

