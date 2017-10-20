#include<stdio.h>
#include<stdlib.h>

typedef struct BiTNode
{
	char data;
	struct BiTNode *lchild,*rchild;
}BiTNode,*BiTree;

BiTree CreateBiTree()
{
	int num;
	BiTree root = NULL;
	scanf("%d",&num);
	if(num != -1)
	{
	
		root = (BiTree)malloc(sizeof(BiTNode));
		root->data = num;
		root->lchild = CreateBiTree();
		root->rchild = CreateBiTree();
	}
	else
	{
		root = NULL;
	}
	return root;
}

void PreOrderTraverse(BiTree T)
{
	if(T != NULL)
	{
		printf("%d ",T->data);
		PreOrderTraverse(T->lchild);
		PreOrderTraverse(T->rchild);
	}
}

void InOrderTraverse(BiTree T)
{
	if(T != NULL)
	{
		PreOrderTraverse(T->lchild);
		printf("%d ",T->data);
		PreOrderTraverse(T->rchild);
	}
}

void PostOrderTraverse(BiTree T)
{
	if(T != NULL)
	{
		PreOrderTraverse(T->lchild);
		PreOrderTraverse(T->rchild);
		printf("%d ",T->data);
	}
}

void DistoryTree(BiTree T)
{
	if(T != NULL)
	{
		DistoryTree(T->lchild);
		DistoryTree(T->rchild);
		free(T);
	}
}

int main()
{
	BiTree tree = NULL;
	int i;
	tree = CreateBiTree();
	//PreOrderTraverse(tree);
	//InOrderTraverse(tree);
	PostOrderTraverse(tree);
	printf("\n");
	DistoryTree(tree);
	return 0;
}
