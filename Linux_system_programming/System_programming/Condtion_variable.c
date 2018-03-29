#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

#define LINE __LINE__
#define BROADCAST 1

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

#if BROADCAST

void *func1(void *arg)
{
	int x = 10;
	pthread_mutex_lock(&mut);
	printf("func1,line:%d\n",LINE);
	while(x > 0)
	{
		printf("%d\n",x--);
		sleep(1);
	}
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mut);
}


#else

void *func1(void *arg)
{
	int x = 10;
	pthread_mutex_lock(&mut);
	printf("func1,line:%d\n",LINE);
	while(x > 0)
	{
		printf("%d\n",x--);
		sleep(1);
		if(x == 7)
		{
			pthread_cond_signal(&cond);
		}
		if(x == 3)
		{
			pthread_cond_signal(&cond);
		}
	}
	pthread_mutex_unlock(&mut);
}

#endif

void *func2(void *arg)
{
	pthread_mutex_lock(&mut);
	printf("func2,line:%d\n",LINE);
	pthread_cond_wait(&cond,&mut);
	printf("func2 released,line:%d\n",LINE);
	pthread_mutex_unlock(&mut);
}

void *func3(void *arg)
{
	pthread_mutex_lock(&mut);
	printf("func3,line:%d\n",LINE);
	pthread_cond_wait(&cond,&mut);
	printf("func3 released,line:%d\n",LINE);
	pthread_mutex_unlock(&mut);
}

int main()
{
	void *status;
	pthread_t t1,t2,t3;
	
	pthread_create(&t1,NULL,func1,NULL);
	pthread_create(&t2,NULL,func2,NULL);
	pthread_create(&t3,NULL,func3,NULL);

	pthread_join(t1,(void *)&status);
	pthread_join(t2,(void *)&status);
	pthread_join(t3,(void *)&status);

	pthread_cond_destroy(&cond);

	return 0;
}