#include <stdio.h>
#include <pthread.h>

static int counter = 0;
struct node 
{ 
	int key; 
	struct node *left, *right; 
}; 

typedef struct Stack {
	struct node *node;
	struct stack *next;
}stack;

pthread_mutex_t mut;
pthread_mutex_t prod;

stack *head = NULL ;

#define max 128

#if 0

stack *rear = NULL ;
stack *dequeue(stack **h)
{
	return pop(h);
}
stack *enqueue(stack **h)
{
	pthread_mutex_lock(&prod);
	stack *head = *h;

	if (overflow()){
		printf("cant push\n");
		pthread_mutex_unlock(&prod);
		return 1;
	}

	stack *tmp = alloc_node(data);

	if (*h == NULL) {
		*h = tmp;
	} else {
		rear->next = tmp;
	}

	tmp->data = data;
	tmp->next = NULL;		
	rear = tmp;
	pthread_mutex_unlock(&prod);
	return rear ;

}
#endif
static stack *alloc_node(struct node *n)
{
	stack *tmp = malloc(sizeof (stack));
	tmp->node = n;
	tmp->next = NULL;
}

void show(stack *head)
{
	#if 0
	for(; head ; head = head->next)
		printf ("%d \n", head->data);
	#endif
}

int underflow()
{
	pthread_mutex_lock(&mut);
	if (counter == 0) {
		pthread_mutex_unlock(&mut);
		return 1;
	}else{
		counter--;
	}
	pthread_mutex_unlock(&mut);

	return 0;
}

int overflow()
{
	pthread_mutex_lock(&mut);
	if (counter == (max-1)) {
		pthread_mutex_unlock(&mut);
		return 1;
	}else{
		counter ++;
	}
	pthread_mutex_unlock(&mut);
	return 0;
}
struct node *pop(stack **h)
{
	pthread_mutex_lock(&prod);
	if (underflow() || (*h == NULL)) {
		pthread_mutex_unlock(&prod);
		printf ("cant pop\n");
		return NULL;
	}

	stack *tmp = *h;
	struct node *tmp1=  (*h)->node;
	*h = (*h)->next;
	free(tmp);
	tmp = NULL;
	pthread_mutex_unlock(&prod);
	return tmp1;
}

int push(stack **h, struct node *n)
{
	pthread_mutex_lock(&prod);
	stack *head = *h;

	if (overflow()){
		printf("cant push\n");
		show(head);
		pthread_mutex_unlock(&prod);
		exit(1);
		return 1;
	}

	stack *tmp = alloc_node(n);
	tmp->next = head;

	*h = tmp;
	pthread_mutex_unlock(&prod);
	return 0;
}

int iterInorderRev(struct node *h)
{
	struct node *current = h;
	struct node *tmp = NULL ;
	if (NULL == h) 
		return NULL ;
	while(1) {
		while(current) {
			push(&head, current);
			current = current->right;
		}
		tmp = pop(&head);	
		if (NULL == tmp)
			break;
		printf ("%d\n", tmp->key );
		current = tmp->left;
	}
		
	return 0;
}
int iterInorder(struct node *h)
{
	iterInorderRev(h);
	struct node *current = h;
	struct node *tmp = NULL ;
	if (NULL == h) 
		return NULL ;
	while(1) {
		while(current) {
			push(&head, current);
			current = current->left;
		}
		tmp = pop(&head);	
		if (NULL == tmp)
			break;
		printf ("%d\n", tmp->key );
		current = tmp->right;
	}
		
	return 0;
}
