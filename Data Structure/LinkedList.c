#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

typedef struct LNode
{
	int data;
	struct LNode *next;
}LNode,*LinkList;

void CreateList(LinkList *L)
{
	*L = (LinkList)malloc(sizeof(LNode));
	if(L == NULL)
	{
		printf("malloc error\n");
		exit(-1);
	}
}

void InsertListR(LinkList L)//尾插法
{
	int x;
	LinkList p;
	p = L;
	scanf("%d",&x);
	while(x != -1)
	{
		p->next = (LinkList)malloc(sizeof(LNode));
		p = p->next;
		p->data = x;
		scanf("%d",&x);
	}
	p->next = NULL;
}

void InsertListF(LinkList L)//头插法
{
	int x;
	LinkList s;
	L->next = NULL;
	scanf("%d",&x);
	while(x != -1)
	{
		s = (LinkList)malloc(sizeof(LNode));
		s->data = x;
		s->next = L->next;
		L->next = s;
		scanf("%d",&x);
	}
}

void ReadListData(LinkList L)
{
	LinkList p;
	p = L;
	p = p->next;
	while(p != NULL)
	{
		printf("%d ",p->data);
		p = p->next;
	}
	printf("\n");
}

void ReadListAddress(LinkList L)
{
	LinkList p;
	p = L;
	p = p->next;
	while(p != NULL)
	{
		printf("%p ",&p->data);
		p = p->next;
	}
	printf("\n");
}

void InsertAfter(LinkList L,int x)
{
	LinkList s;
	s = (LinkList)malloc(sizeof(LNode));
	s->data = x;
	s->next = L->next;
	L->next = s;
}

int Delete(LinkList L,int num)
{
	LinkList p1 = L;
	LinkList p2 = NULL;
	int flag = 0;
	if(p1 == NULL)
	{
		printf("list is empty\n");
		exit(-1);
	}
	if(p1->data == num)
	{
		L = L->next;
		free(p1);
		p1->data = 0;
		flag = 1;
		return flag;
	}
	else
	{
		while(p1 != NULL)
		{
			p2 = p1;
			p2 = p2->next;
			if(p2->data == num)
			{
				p1->next = p2->next;
				free(p2);
				flag = 1;
				break;
			}
			p1 = p1->next;
		}
		return flag;
	}
}

void DestroyList(LinkList L)
{
	LinkList p;
	p = L;
	while(L)
	{
		p = L->next;
		free(p);
		L = p;
	}
}

int main()
{
	int data;
	LinkList list;
	CreateList(&list);
	//InsertListF(list);
	//ReadListData(list);
	//ReadListAddress(list);
	InsertListR(list);
	ReadListData(list);
	ReadListAddress(list);
	//scanf("%d",&data);
	//while(data != -1)
	//{
	//	InsertAfter(list,data);
	//	scanf("%d",&data);
	//}
	//ReadListData(list);
	printf("%d\n",Delete(list,1));
	ReadListData(list);
	ReadListAddress(list);
	DestroyList(list);
	return 0;
}
