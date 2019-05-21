#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>

extern int status_g ;

struct LIST {
	int guy ;
	struct LIST* next ;
};

typedef struct LIST list;

enum by {
	BYPOS,
	BYVAL,
	INVALID,
};

int put (list **head, int member);
int delete (list **head, int member, enum by);
list* search (list *head, int member, enum by);
int check_failure(void);
int easy_swap (list*, int, int,enum by);
int alloc_item ( list **client, int data);
int _val (char* passed_string) ;
void show (list **head);

#ifdef DEBUG
#define ERROR_P(...) fprintf (stderr,...)
#define INFO_P(...) fprintf (stdout,...)
#endif
