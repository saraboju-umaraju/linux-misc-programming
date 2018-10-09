#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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
	int l = 0;
	int sum = 0;
	
	for (;l<strlen (name);l++)
		sum += name[l];
	return sum;
}
void traverse(struct node *head)
{
	while(head) {
		printf("%s ",head->name);
		head = head->next;
	}
}
int main(void)
{
	int n = 0;
	int how_many = 7;
	char buf[30] = {};
	struct hash **table = calloc(how_many,sizeof(struct hash*));
	/*validation is mandatory */
	int l=0;
	for(;l<how_many;l++){
		table[l] = calloc(1,sizeof(struct hash));
		/*validation is mandatory */
		table[l]->head = NULL;
	}
	
	printf("how many names: \n");
	scanf("%d",&n); /* I KNOW THIS IS SUPER BAD */
	int index = -1;
	__fpurge(stdin);
	while(n){
		fgets(buf,29,stdin);
		buf[strlen(buf)-1] = '\0';
		index = calculate_hash(buf)%how_many;
		insert(buf,&table[index]->head);
		table[index]->count++;
		memset(buf,'\0',30);
		n--;
	}

	for(l=0;l<how_many;l++){
		printf("in hash table %d :",l);
		traverse(table[l]->head);
		printf("\n",table[l]->count);
	}		

	return 0;
}
