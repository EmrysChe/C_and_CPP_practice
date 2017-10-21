#include<stdio.h>
#include<stdlib.h>

int cmp(const void *a,const void *b)
{
	return *(int *)a - *(int *)b;  //升序排序
	//return *(int *)b - *(int *)a; //降序排序
}

int NormalSearch(int a[],int n,int data)
{
	int i;
	int flag = 0;
	for(i = 0;i < n;i++)
	{
		if(a[i] == data)
			return i;
	}
	return -1;
}

int BinarySearch(int a[],int n,int data)
{
	int left = 0;
	int right = n - 1;
	int mid = (right - left) / 2;
	while(left <= right)
	{
		mid = (right + left) / 2;
		if(a[mid] < data)
		{
			left = mid + 1;
		}
		else if(a[mid] > data)
		{
			right = mid - 1;
		}
		else
		{
			return mid;
		}
	}
	return -1;
}

int main()
{
	int i;
	int flag = 0;
	int a[10] = {33,20,4,1,9,0,7,21,13,11};
	qsort(a,10,sizeof(a[0]),cmp);
	/*for(i = 0;i < 10;i++)
	{
		printf("%d ",a[i]);
	}
	printf("\n");*/
	//flag = NormalSearch(a,10,11);
	flag = BinarySearch(a,10,11);
	if(flag >= 0)
		printf("%d\n",flag);
	else
		printf("data not exist\n");

	return 0;
}
