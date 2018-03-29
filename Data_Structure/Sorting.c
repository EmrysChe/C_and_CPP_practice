#include<stdio.h>
#include<stdlib.h>

void InsertionSort(int a[],int n)//插入排序
{
	int i,j,tmp;
	for(i = 1;i < n;i++)
	{
		tmp = a[i];
		for(j = i - 1;j >=0 && a[j] > tmp;j--)
			a[j+1] = a[j];
		a[j+1] = tmp;
	}
}

void SelectionSort(int a[],int n)//选择排序
{
	int i,j,tmp;
	for(i = 0;i < n;i++)
		for(j = i + 1;j < n;j++)
		{
			if(a[j] < a[i])
			{
				tmp = a[j];
				a[j] = a[i];
				a[i] = tmp;
			}
		}
}

void BubbleSort(int a[],int n)//冒泡排序
{
	int i,j,tmp;
	for(i = 0;i < n;i++)
	{
		for(j = i;j < n - 1;j++)
		{
			if(a[j] > a[j+1])
			{
				tmp = a[j];
				a[j] = a[j+1];
				a[j+1] = tmp;
			}
		}
	}
}

void ShellSort(int a[], int n)//希尔排序
{
    int i,j,gap;
    for(gap = n / 2;gap > 0;gap /= 2)
    {
        for(i = 0;i < gap;i++)
        {
            for(j = i + gap;j < n;j += gap) 
            {
                if(a[j] < a[j-gap])
                {
                    int tmp = a[j];
                    int k = j - gap;
                    while(k >= 0 && a[k] > tmp)
                    {
                        a[k+gap] = a[k];
                        k -= gap;
                    }
                    a[k+gap] = tmp;
                }
            }
        }
    }
}

int main()
{
	int i;
	int a[10] = {4,1,4,5,8,9,8,7,8,11};
	//InsertionSort(a,10);
	//SelectionSort(a,10);
	//BubbleSort(a,10);
	ShellSort(a,10);
	for(i = 0;i < 10;i++)
	{
		printf("%d ",a[i]);
	}
	printf("\n");
	return 0;
}
