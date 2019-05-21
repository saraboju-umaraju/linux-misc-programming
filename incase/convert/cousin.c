#include <stdio.h>
struct node 
{ 
	int data; 
	struct node *left, *right; 
}; 

int depthutil(struct node *root,int data, int level)
{
	if (root == NULL)
		return 0;
	if (root->data == data)
		return level;
	int l = depthutil(root->left,data, level+1);
	if (l)
		return l;
	l = depthutil(root->right,data, level+1);
	return l;
}

int fdepth(struct node *root,int data)
{
	return depthutil(root,data,1);
}	

int print_at_level(struct node *root, int level, int data)
{
	if (NULL == root || level <2)
			return;
	if (level == 2) {
		if ( (root->left && root->left->data == data) || (root->right && root->right->data == data)) return;
			if(root->left)
				printf("%d\n", root->left->data);
			if(root->right)
				printf("%d\n", root->right->data);
	}
	else if (level > 1) {
		print_at_level(root->left, level-1, data);	
		print_at_level(root->right, level-1, data);	
	}
}
int print_cousins (struct node *root,int data)
{
	int depth = fdepth(root,data);
	printf("depth is %d\n",depth-1);
	print_at_level(root,depth, data);
	return 0;
}

