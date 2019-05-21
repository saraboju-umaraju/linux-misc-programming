#include <stdio.h>
#include <pthread.h>

static int counter = 0;
struct node 
{ 
	int key; 
	struct node *left, *right; 
}; 

typedef struct Stack {
	struct node *node ;
	struct queue *next;
}queue;

pthread_mutex_t mut;
pthread_mutex_t prod;
#define max 15

queue *rear = NULL ;

queue *head = NULL ;

queue *alloc_node(struct node *node)
{
	queue *tmp = malloc(sizeof (queue));
	tmp->node = node;
	tmp->next = NULL;
}

struct node* pop(queue **h);
struct node *dequeue(queue **h)
{
	return pop(h);
}

int insert_end(queue **h, struct node *node)
{
	queue *head = *h;
	
	queue *tmp = alloc_node(node);
	
	if (NULL == (*h) || NULL == rear) {
		*h = rear = tmp ;
	}else{
		rear->next = tmp;
		rear = tmp;
	}
	return 0;
		
}
queue *enqueue(queue **h,struct node *node)
{
	if (overflow()) {
		printf ("H\n");
		return 1;
	}
	insert_end(h,node);
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
struct node* pop(queue **h)
{
	pthread_mutex_lock(&prod);
	if (underflow() || (rear == NULL)) {
		pthread_mutex_unlock(&prod);
		printf ("P\n");
		return -1;
	}

	queue *tmp = *h;
	struct node *data =  (*h)->node;
	*h = (*h)->next;
	if (NULL == (*h) )
		rear = NULL;
	free(tmp);
	tmp = NULL;
	pthread_mutex_unlock(&prod);
	return data;
}

#if 0
1) Create an empty queue q
2) temp_node = root /*start from root*/
3) Loop while temp_node is not NULL
    a) print temp_node->data.
    b) Enqueue temp_node’s children (first left then right children) to q
    c) Dequeue a node from q and assign it’s value to temp_node
#endif
int level_order(struct node *root)
{
	if (root == NULL)
		return 1;

	struct node *current = root;
	struct node *tmp = NULL;

	while(current) {
		printf ("%d\n",current->key);
		enqueue(&head,current->left);
		enqueue(&head,current->right);
		current = dequeue(&head);
	}

	return 0;
}
