/*whole goal is to represent a graph below*/
/*
0-->1 0-->4
1-->2 1-->3 1-->4
2-->3 3-->4
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 7

struct NODE {
	int node;
	struct NODE *next;
};

typedef struct NODE node;

struct NODELIST{
	struct NODE *head;
};

typedef struct NODELIST graph;

void _dfs(bool *v, graph *root, int start)
{
	bool flag = true ;
	node *tmp = NULL;
	int l = 0;
	for(; l<N; l++){
                flag = v[l] && flag;
        }
	if (flag == false){
		v[start] = true;
		for(tmp = root[start].head; tmp ; tmp = tmp -> next )
			_dfs(v,root,tmp->node);
	}else{
		return;
	}
	
}

void dfs(graph *root)
{
	bool *visited = malloc(sizeof(bool)*N);
	int l = 0;
	for(; l<N; l++){
		visited[l] = false;
	}
	_dfs(visited, root,5);
	l = 0;
	for(; l<N; l++){
		printf ("%d ",visited[l]);
	}
	puts("");
}

void print(node *head)
{
	for(; head ; head = head->next)
		printf ("%d ",head->node);
}

void print_edges(graph *root,int node)
{
	printf ("edges of node %d are :", node);
	print (root[node].head);
	puts("");
}

void display(graph *root)
{
        int l = 0;
        for(; l<N; l++){
        	printf("%d :", l);
	        print (root[l].head);
		puts("");
	}
}

void insert(node **head, int data)
{
	node *v = malloc(sizeof(node));
	v->node = data;
	v->next = *head;
	*head = v;
	
}

void add_edge(graph *root, int from, int to)
{
	insert(&root[from].head,to);
}

int main (void)
{
	graph *list = (graph*)malloc(sizeof(list)*N);
	/*validate the allocation*/

	int l = 0;
	for(; l<N; l++)
		list[l].head = NULL;

	add_edge(list,0,1); 

	add_edge(list,0,4); 

	add_edge(list,1,2); 

	add_edge(list,1,3); 

	add_edge(list,1,4); 

	add_edge(list,2,3); 

	add_edge(list,3,4); 
	
	add_edge(list,5,6); 

//	display(list);
	
	//print_edges(list, 0);

	dfs(list);
}
