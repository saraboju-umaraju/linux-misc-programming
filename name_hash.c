#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio_ext.h>

int _val (char* passed_string) ;

struct node {
	char name[30] ; /* can optimize it but it's ok for now */
	struct node *next;
};

struct hash {
    struct node *head;
    int count;
};


void insert(char *name, struct node **head)
{
	struct node *new = malloc(sizeof(struct node));
	strcpy(new->name,name);  /* bad bad */
	new->next = *head;
	*head = new;
}
int calculate_hash(char *name)
{
	if (name == NULL )
		return -1;

	return (name[0] - 'a')%25 ;
}
void traverse(struct node *head)
{
	while(head) {
		printf("%s ",head->name);
		head = head->next;
	}
}
int find( struct node *head, char *name)
{
	while(head) {
		if ( !strcmp(name,head->name))
			return 0;
		head = head->next;
	}
	return -1;
	
}
int main(void)
{
	int n = 0;
	int how_many = 26;
	int index = -1;
	char buf[30] = {};
	int l=0;
	int tmp = 0;

	struct hash **table = calloc(how_many,sizeof(struct hash*));

	if ( NULL == table ) {
		printf ("You've ran out of memory\n");
		return -ENOMEM ;
	}
	
	for(;l<how_many;l++){
		table[l] = calloc(1,sizeof(struct hash));

		if ( NULL == table[l] ) {

			for(tmp=0;tmp<l;tmp++)
				free(table[tmp]) ;
			free(table);

			printf ("You've ran out of memory\n");
			return -ENOMEM ;
			
		}
		table[l]->head = NULL;
	}
	
	n = _val("how many names\n");

	__fpurge(stdin);

	while(n){
		fgets(buf,29,stdin);
		/** validation is needed */	
		buf[strlen(buf)-1] = '\0';
		index = calculate_hash(buf);
		if ( index == -1 )
			continue ;
		insert(buf,&table[index]->head);
		table[index]->count++;
		memset(buf,'\0',30);
		n--;
	}

	for(l=0;l<how_many;l++){
		if ( NULL == table[l]->head )
			continue ;
		printf("In hash table %d :",l);
		traverse(table[l]->head);
		printf("\n");
	}		

	do {
		puts ("which name");
		__fpurge(stdin);
		fgets(buf,29,stdin);
		/** validation is needed */	
		buf[strlen(buf)-1] = '\0';
		tmp = find(table[calculate_hash(buf)]->head,buf);
		if (!tmp) {
			printf ("name present\n");
		} else {
			printf ("double check string\n");
	}
	__fpurge(stdin);
	}while (1);

	return 0;
}
