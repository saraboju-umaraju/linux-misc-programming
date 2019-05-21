#include <stdio.h>

struct node
{
    int data;
    struct node *left, *right;
};

int sumofchildren (struct node *root)
{
	if (NULL == root || (root->right == NULL && root->left == NULL)) {
		return 1;
	}	

	int sum = 0;
	if (root->right)
			sum+= root->right->data;
	if (root->left)
			sum+= root->left->data;
	
	printf ("sum %d, %d\n", sum,root->data);

	
	return ((sum == root->data) && sumofchildren(root->left) && sumofchildren(root->right));
}
