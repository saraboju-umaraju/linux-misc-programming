#include <stdio.h>
#include <pthread.h>

static int counter = 0;

typedef struct Stack {
	int data;
	struct queue *next;
}queue;

pthread_mutex_t mut;
pthread_mutex_t prod;

#define max 15

queue *rear = NULL ;

queue *head = NULL ;

queue *alloc_node(int data)
{
	queue *tmp = malloc(sizeof (queue));
	tmp->data = data;
	tmp->next = NULL;
}

queue *dequeue(queue **h)
{
	return pop(h);
}

int insert_end(queue **h, int data)
{
	queue *head = *h;
	
	queue *tmp = alloc_node(data);
	
	if (NULL == (*h) || NULL == rear) {
		*h = rear = tmp ;
	}else{
		rear->next = tmp;
		rear = tmp;
	}
	return 0;
		
}
queue *enqueue(queue **h,int data)
{
	if (overflow()) {
		printf ("H\n");
		return 1;
	}
	insert_end(h,data);
}

void show(queue *head)
{
	for(; head ; head = head->next)
		printf ("%d \n", head->data);
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
int pop(queue **h)
{
	pthread_mutex_lock(&prod);
	if (underflow() || (rear == NULL)) {
		pthread_mutex_unlock(&prod);
		printf ("P\n");
		return -1;
	}

	queue *tmp = *h;
	int data =  (*h)->data;
	*h = (*h)->next;
	if (NULL == (*h) )
		rear = NULL;
	free(tmp);
	tmp = NULL;
	pthread_mutex_unlock(&prod);
	printf("pop %d\n",data);
	return data;
}

void *f_two () 
{
	printf("%d\n",1);
	while(1){
		dequeue(&head);
	}
}

void *f_one ()
{
	printf("%d\n",1);
	while(1){
		enqueue(&head, rand()%100);
	}
}

#define COUNT 10

int main (int argc, char *argv[])
{
	int tmp = 14;

	srand(getpid());

	pthread_t one;

	pthread_t two[COUNT];

	pthread_mutex_init(&mut, NULL);

	pthread_mutex_init(&prod, NULL);

	pthread_create ( &one , NULL , f_one , NULL ) ;

	int l = 0;
	for(l = 0; l < COUNT; l++)
		pthread_create ( &two[l] , NULL , f_two , NULL ) ;
	
	printf("created threads\n");
	
	pthread_join(one,NULL);

	for(l = 0; l < COUNT; l++)

		pthread_join(two[l],NULL);

	show(head);

	return 0;
}

