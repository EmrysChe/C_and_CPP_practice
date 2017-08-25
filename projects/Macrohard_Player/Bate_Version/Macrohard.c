/*********************************************************************************
  *Copyright(C),Team Macrohard
  *FileName:Macrohard.c
  *Author:Emrys
  *Version:1.170722
  *Date:2017.07.22
  *Description: main function file
  *Function List:
  	01.int main(void);main function
**********************************************************************************/
#include "basic.h"

extern Song *pnow;
extern Song *plast;
extern int clsthread;	//关闭线程
extern int flagpause;
extern int fdpp[2];	//无名管道
extern int flag;	//显示数据标志	1：显示  0：不显示
extern sem_t sem;

int main(void)
{
	memset(filename,'\0',sizeof(filename));
	printf("请输入文件夹路径:");
	scanf("%s",filename);
	List_head *linklist = getSongList(filename);
	nodePrint(linklist);
	pnow = linklist->head;
	lrclist();
	char name[NAMESIZE] = {0};
	strcat(name,filename);
	strcat(name,pnow->name);
	
	int ret = sem_init(&sem,0,1);
	if(ret == -1)
		perror("信号量初始化失败");
	
	if(access(FIFO,F_OK) == 0)
	{
		unlink(FIFO);
		mkfifo(FIFO,0777);
	}
	else
	{
		mkfifo(FIFO,0777);
	}

	if(pipe(fdpp) < 0)
		perror("无名管道创建失败");
	
	fcntl(fdpp[0],F_SETFL,O_NONBLOCK);

	pid_t pid = fork();
	if(pid < 0)
		perror("创建子进程失败");
	else if(pid == 0)
	{	
		close(fdpp[0]);
		dup2(fdpp[1],1);
		execlp("mplayer","mplayer","-slave","-quiet","-idle","-input","file=fifo",name,NULL);
	}
	else
	{
		close(fdpp[1]);
		sleep(1);
		
		pthread_t get_tid1,get_tid2,get_tid3;
		ret = 0;
		
		ret = pthread_create(&get_tid1,NULL,getCmdThread,(List_head *)linklist);
		if(ret != 0)
			perror("获取按键输入命令线程创建失败");
		
		ret = pthread_create(&get_tid2,NULL,dataCmdThread,NULL);
		if(ret != 0)
			perror("定时发送命令获取数据线程创建失败");

		ret = pthread_create(&get_tid3,NULL,readThread,(List_head *)linklist);
		if(ret != 0)
			perror("读取数据处理线程创建失败");
					
		pthread_join(get_tid1,NULL);
		pthread_join(get_tid2,NULL);
		pthread_join(get_tid3,NULL);
		sem_destroy(&sem);
		listDestroy(linklist);
	}
	return 0;
}
