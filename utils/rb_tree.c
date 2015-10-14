/*************************************************************************
	> File Name: rb_tree.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2014年11月21日 星期五 19时03分16秒
 ************************************************************************/

#include<stdio.h>
#include<malloc.h>
enum color {
	RED ,BLACK 
};
struct node {
	int value;
	struct node *left;
	struct node *right;
	struct node *father;
	enum color color; 
};
struct tree{
	struct node *root;
	struct node *nil;
};

struct tree*  init_rb_tree()
{
	struct tree *tree;
	tree = (struct tree *)malloc(sizeof(struct tree));
	if (!tree)
		return NULL;
	tree->nil = (struct node *)malloc(sizeof(struct node));
	if(!tree->nil){
		free(tree);
		return NULL;
	}
	 tree->nil->color = BLACK;
	//tree->nil->father = tree->nil;
	//tree->nil->left   = tree->nil;
	//tree->nil->right  = tree->nil;
	tree->root = tree->nil;
}
void free_node(struct tree *tree,struct node *node){
	struct node *left,*right;
	if (node == tree->nil)
		return;
	left = node->left;
	right = node->right;
	free(node);
	free_node(tree,left);
	free_node(tree,right);
}
void free_tree(struct tree *tree){
	free_node(tree,tree->root);
	free(tree->nil);
	free(tree);
}

void left_rotate(struct tree *tree,struct node *x)
{
	struct node *y;
	y = x->right;
	x->right = y->left;
	if(y->left != tree->nil)
		y->left->father = x;
	
	y->father = x->father;
	if(x->father == tree->nil)
		tree->root = y;
	else if(x->father->left == x)
		x->father->left = y;
	else 
		x->father->right = y;
	
	x->father = y;
	y->left	  = x;
	
}

void right_rotate(struct tree *tree,struct node *x)
{
	struct node *y;
	y = x->left;

	x->left = y->right;
	if(y->right != tree->nil)
		y->right->father = x;


	y->father = x->father ;
	if(x->father == tree->nil)
		tree->root = y;
	else if (x->father->left == x)
		x->father->left = y;
	else 
		x->father->right = y;

	x->father = y ;
	y->left = x;
}

struct node* rb_find(struct tree *tree,int value)
{
	struct node *node;
	node = tree->root;
	
	while(node->value != value && node != tree->nil){
		if(value < node->value)
			node = node ->left;
		else 
			node = node->right;
	}
	if (node == tree->nil){
		printf("no node %d\n",value);
		return NULL;
	}
	return node ;
}

void rb_insert_fixup(struct tree *tree,struct node *x)
{
	struct node *uncle;
	while(x->father->color == RED){
		if (x->father->father->left == x->father){
				uncle = x->father->father->right;

				if(uncle->color == RED){
						x->father->father->color = RED;
						uncle->color =	BLACK;	
						x->father->color =  BLACK;
						x = x->father->father;
				}
				else if(x->father->right == x){
					
					left_rotate(tree,x->father);
					x = x->left;	
				}
				else {
					x->father->color = BLACK;
					x->father->father->color  = RED ;
					right_rotate(tree,x->father->father);
				}

		}
		else {
			uncle = x->father->father->left;
			if(uncle->color == RED){
				uncle->color = BLACK;
				uncle->father->color = RED;
				x->father->color = BLACK;
				x = uncle->father;
			}

			else if(x->father->left == x){
				x=x->father;
				right_rotate(tree,x);
			}
			else {
				x->father->color = BLACK;
				x->father->father->color = RED;
				left_rotate(tree,x->father->father);
			}
		}
	}

	tree->root->color = BLACK;
}


void rb_insert(struct tree *tree,int value)
{
	struct node *node,*pre;
	struct node *x;
	x=(struct node*)malloc(sizeof(struct node));
	node = tree->root;
	x->color = RED;
	x->left = tree->nil;
	x->right = tree->nil;
	x->father =  tree->nil;
	x->value  = value;
	pre = tree->nil;	
	while(node != tree->nil){
		pre =  node;
		if(x->value < node->value)
			node = node ->left;
		else 
			node = node ->right;
		}

	x->father = pre;
	if(pre == tree->nil)
		tree->root = x;
	else if(x->value < pre->value )
		pre->left =  x;
	else 
		pre->right = x;
	rb_insert_fixup(tree,x);
	
//	printf("x->father->value:%d\n",x->father->value);

}

struct node* tree_min(struct tree *tree,struct node *node)
{
	struct node *pre;
	pre = tree->nil;
	while(node != tree->nil){
		pre = node;
		node = node ->left;
	}
	return pre;
}

void rb_delete_fixup(struct tree *tree , struct node *fix )
{
	struct node *brother;
	while (fix != tree->root && fix->color == BLACK ){
		if(fix->father->left == fix){
			brother = fix->father->right;

			if(brother->color == BLACK && brother->left->color == BLACK && brother->right->color == BLACK){
				brother->color = RED;
				fix = fix->father;
			}
			else if(brother->color == BLACK && brother->right->color == RED){
				brother->color = fix->father->color;
				fix->father->color = BLACK;
				brother->right->color = BLACK;
				left_rotate(tree,fix->father);
				fix = tree->root;
			}
			else if(brother->color == BLACK ){
				brother->color = RED;
				brother->left->color = BLACK;
				right_rotate(tree,brother);
				
			}
			else {
				brother->color = BLACK;
				brother->father->color = RED;
				left_rotate(tree,fix->father);

			}
		}
		else{
			brother = fix->father->left;
			if(brother->color == BLACK && brother->left->color ==BLACK && brother->right->color == BLACK){
				brother->color = RED;
				fix = fix->father;
			}
			else if(brother->color ==BLACK && brother->left->color == RED){
				brother->color = fix->father->color;
				fix->father->color == BLACK;
				brother->left->color == BLACK;
				right_rotate(tree,brother->father);
				fix = tree->root;
			}
			else if(brother->color == BLACK ){
				brother->color == RED;
				brother->right->color == BLACK;
				left_rotate(tree,brother);
			}
			else {
				brother->color ==  BLACK;
				brother->father->color == RED;
				right_rotate(tree,brother->father);
			}
		}

	}

	fix->color = BLACK;
}
void rb_delete(struct tree *tree,struct node *x)
{
	enum color color;
	struct node *repx,*fix;
	
	if(x->left == tree->nil){
		repx = x->right;
		color = x->color;
		fix = repx;
		
		repx->father = x->father;
		if(x->father == tree->nil)
			tree->root = repx;
		else if(x->father->left == x)
			x->father->left = repx;
		else 
			x->father->right = repx;
		
	}
	else if(x->right == tree->nil){
		repx = x->left;
		color = x->color;
		fix  = repx;

		repx->father = x->father;
		if(x->father == tree->nil)
			tree->root = repx;
		else if(x->father->left == x)
			x->father->left = repx;
		else 
			x->father->right = repx;
	}
	else {
		repx = tree_min(tree,x->right);
		color = repx->color;
		fix = repx->right;
	
		
		fix->father = repx->father;
		if(repx->father->left == repx)
			repx->father->left = fix;
		else
			repx->father->right = fix;
		

		repx->father =x->father;
		if(x->father == tree->nil)
			tree->root = repx;
		else if(x->father->left == x)
			x->father->left = repx;
		else 
			x->father->right = repx;
	
		repx->left = x->left;
		x->left->father = repx;
		repx->right = x->right;
		x->right->father = repx;
		
		repx->color = x->color;

	
	}

if (color == BLACK)
	rb_delete_fixup(tree,fix);


}

int rb_check(struct tree *tree,struct node *node,int high)
{
	int left,right;
	if (node->color == RED && (node->left->color == RED || node->right->color ==RED ))
		return -1;
	if (node == tree->nil)
		return high+1;
	if (node->color == BLACK){
		left = rb_check(tree,node->left,high+1);
//		printf("left:%d\n",left);
		if (left == -1)
			return -1;
		right = rb_check(tree,node->right,high+1);
//		printf("right:%d\n",right);
		if (right == -1)
			return -1;
		if(left !=right)
			return -1;
		return left;
	}
	if(node->color == RED)	{	
		left = rb_check(tree,node->left,high);
//	//	printf("left:%d\n",left);
		if (left == -1)
			return -1;
		right = rb_check(tree,node->right,high);
//		printf("right:%d\n",right);
		if (right == -1)
			return -1;
		if(left !=right)
			return -1;
		return left;
	}
}
bool  check(struct tree *tree)
{
	struct node *node;
	 int  high = 0;
	if(tree->root->color == RED)
		return false;
	node = tree->root;
	while(node != tree->nil){
		if(node->color == BLACK)
			high++;
		else if (node->father->color == RED)
			return false;
		node = node->left;
		if (node == tree->nil) ;//printf("sdfsd\n");
	}
	
	printf("high ==%d\n",high);
	if (rb_check(tree,tree->root,0)!= -1)
		return true;
	return false;
	
	
}
void myprintf(struct tree* tree,struct node* node)
{
	int i;
	if (node == tree->nil)
		return;
	
	printf("%d ",node->value);
	if (node->color ==RED)
		printf("R");
	else
		printf("B");
	printf("\n");
	myprintf(tree,node->left);
	myprintf(tree,node->right);
}
int  main()
{
	struct tree *tree;
	struct node *node;
	int fun,value,i;
	tree = init_rb_tree();
	while(1){
		printf("function:");
		scanf("%d",&fun);
		if (fun == 1){
			printf("add node value:");
			scanf("%d",&value);
			for(i=1;i<=value;i++)
				rb_insert(tree,i);
		}
		if (fun == 2){
			printf("delect node value:");
			scanf("%d",&value);
			node = rb_find(tree,value);
			if(node )
			rb_delete(tree,node);
		}
		if (fun == 3){
			goto end;
		}
	//	myprintf(tree,tree->root);
		if (check(tree))
			printf("true\n");
		else
		        printf("false\n");
	}
end:
	free_tree(tree);
}



