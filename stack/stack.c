#include <stdio.h>
#include <pthread.h>

static int counter = 0;

typedef struct Stack {
	int data;
	struct stack *next;
}stack;

pthread_mutex_t mut;
pthread_mutex_t prod;

#define max 128

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

stack *alloc_node(int data)
{
	stack *tmp = malloc(sizeof (stack));
	tmp->data = data;
	tmp->next = NULL;
}

void show(stack *head)
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
int pop(stack **h)
{
	pthread_mutex_lock(&prod);
	if (underflow() || (*h == NULL)) {
		pthread_mutex_unlock(&prod);
		printf ("cant pop\n");
		return -1;
	}

	stack *tmp = *h;
	int data =  (*h)->data;
	*h = (*h)->next;
	free(tmp);
	tmp = NULL;
	pthread_mutex_unlock(&prod);
	printf("pop %d\n",data);
	return data;
}

int push(stack **h, int data)
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

	stack *tmp = alloc_node(data);
	tmp->next = head;

	*h = tmp;
	printf ("push %d\n",data);
	
	pthread_mutex_unlock(&prod);
	return 0;
}

stack *head = NULL ;

void *f_two () 
{
	printf("%d\n",1);
	while(1){
		pop(&head);
	}
}

void *f_one ()
{
	printf("%d\n",1);
	while(1){
		push(&head, rand()%100);
	}
}


int main (int argc, char *argv[])
{
	int tmp = 14;

	srand(getpid());

	pthread_t one;

	pthread_t two;

	pthread_t three;

	pthread_mutex_init(&mut, NULL);

	pthread_mutex_init(&prod, NULL);

	pthread_create ( &one , NULL , f_one , NULL ) ;

	pthread_create ( &two , NULL , f_two , NULL ) ;

	pthread_create ( &three , NULL , f_two , NULL ) ;
	
	printf("created threads\n");
	
	pthread_join(one,NULL);

	pthread_join(two,NULL);

	show(head);

	return 0;
}

