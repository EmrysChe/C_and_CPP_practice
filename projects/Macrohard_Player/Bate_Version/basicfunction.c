/*********************************************************************************
  *Copyright(C),Team Macrohard
  *FileName:basicfunction.c
  *Author:Emrys/Lu Yulong
  *Version:1.170722
  *Date:2017.07.22
  *Description: basic function of music player
  *Function List:
  	01.List_head *headCreate(void);创建链表头
	02.Song *nodeCreate(char *name);创建结点
	03.void nodeInsertTail(List_head *linklist, Song *pSong);添加结点到链表尾部
	04.void nodePrint(List_head *linklist);打印整张链表
	05.Song *nodeSearch(List_head *linklist, char *name);查找结点数据，返回结点地址，失败返回NULL
	06.void listDestroy(List_head *linklist);销毁整张链表
	07.List_head *getSongList(char *pathname);获得歌曲链表
	08.void blankChange(char *name);带空格的歌曲名转换格式
	09.void changeSong(char *name);切换歌曲
	10.void volumeAdd();增加音量
	11.void volumeSub();减少音量
	12.void muteSet();静音
	13.void seekBack();快退
	14.void seekFront();快进
	15.void msgTrans(char *msg);处理线程数据
	16.void *getCmdThread(void *cmd);获取按键输入命令
	17.void *readThread(void *msg);读取数据并处理
	18.void *dataCmdThread(void *cmd);定时发送命令获取数据
	19.void OutputLyrics(long n);输出歌词
	20.List lyric_list_create(FILE *fp);创建歌词链表
	21.long ms(char origin[9]);
	22.void lrclist();
	23.void lrc_list_destroy();销毁歌词链表
**********************************************************************************/

#include "basic.h"

Song *pnow = NULL;
Song *plast = NULL;
int clsthread = 0;	//关闭线程
int flagpause = -1;	//暂停标志
int fdpp[2];	//无名管道
int flag = 0;	//显示数据标志	1：显示  0：不显示
sem_t sem;		//信号量
List Head = NULL;

List_head *headCreate(void)
{
	List_head *linklist;
	linklist = (List_head *)malloc(sizeof(List_head));
	if(linklist == NULL)
		perror("链表头创建失败");
	
	linklist->head = NULL;
	linklist->song_num = 0;
	
	return linklist;
}

Song *nodeCreate(char *name)
{
	Song *pSong = (Song *)malloc(sizeof(Song));
	if(pSong == NULL)
		perror("节点创建失败");

	pSong->next = NULL;
	pSong->prev = NULL;
	bzero(pSong->name,NAMESIZE);
	strcpy(pSong->name,name);

	return pSong;
}

void nodeInsertTail(List_head *linklist, Song *pSong)
{
	if(linklist->head == NULL)
	{
		linklist->head = pSong;
		linklist->song_num = 1;
	}
	else
	{
		Song *p = linklist->head;
		while(p->next != NULL)
		{
			p = p->next;
		}
		p->next = pSong;
		pSong->prev = p;
		linklist->song_num++;
	}
}

void nodePrint(List_head *linklist)
{
	printf("\n******************************************************\n");
	Song *p = linklist->head;

	printf("歌曲数量:%d\n",linklist->song_num);
	printf("歌曲列表:\n");
	while(p != NULL)
	{
		printf("%s\n",p->name);
		p = p->next;
	}
	printf("\n******************************************************\n");
}

Song *nodeSearch(List_head *linklist, char *name)
{
	Song *p = linklist->head;

	while(p != NULL)
	{
		if(strcmp(p->name,name) == 0)
			return p;
		else
			p = p->next;
	}
	
	return NULL;
}

void listDestroy(List_head *linklist)
{
	Song *p = linklist->head;
	Song *next = NULL;

	while(p != NULL)
	{
		next = p->next;
		p->next = NULL;
		free(p);
		p = next;
	}

	linklist->head = NULL;
	free(linklist);
}

List_head *getSongList(char *pathname)
{
	List_head *linklist = headCreate();
	Song *pSong = NULL;

	DIR *dirp = opendir(pathname);
	if(dirp == NULL)
		printf("该文件夹下不存在音乐文件!\n");

	struct dirent *pfile;
	pfile = readdir(dirp);
	while(pfile != NULL)
	{
		if(strstr(pfile->d_name, ".mp3") != NULL || strstr(pfile->d_name, ".flac") != NULL || 
		strstr(pfile->d_name, ".m4a") != NULL)
		{
			pSong = nodeCreate(pfile->d_name);
			nodeInsertTail(linklist,pSong);
		}
		pfile = readdir(dirp);
	}
	closedir(dirp);
	plast = pSong;

	return linklist;
}

void blankChange(char *name)
{
	char buf[NAMESIZE] = {0};
	char *p = strtok(name," ");
	if(p != NULL)
		strcat(buf, p);
	p = strtok(NULL," ");
	while(p != NULL)
	{
		strcat(buf,"\\ ");
		strcat(buf,p);
		p = strtok(NULL," ");
	}
	strcpy(name,buf);
}

void changeSong(char *name)
{
	printf("正在播放 %s...\n", strstr(name,filename));
	blankChange(name);
	char pathname[NAMESIZE] = {0};
	strcat(pathname,"loadfile ");
	strcat(pathname,name);
	strcat(pathname,"\n");
	
	int fd;
	if((fd = open(FIFO,O_WRONLY | O_NONBLOCK)) < 0)
		perror("打开管道错误");
	write(fd,pathname,strlen(pathname));
	lrc_list_destroy();
	lrclist();
	close(fd);
}

void volumeAdd()
{
	char addcmd[CMDSIZE];
	memset(addcmd,'\0',sizeof(addcmd));
	strcpy(addcmd,"volume 1\n");
	int fd;
	if((fd = open(FIFO,O_WRONLY | O_NONBLOCK)) < 0)
		perror("打开管道错误");
	write(fd,addcmd,strlen(addcmd));
	close(fd);
}

void volumeSub()
{
	char subcmd[CMDSIZE];
	memset(subcmd,'\0',sizeof(subcmd));
	strcpy(subcmd,"volume -1\n");
	int fd;
	if((fd = open(FIFO,O_WRONLY | O_NONBLOCK)) < 0)
		perror("打开管道错误");
	write(fd,subcmd,strlen(subcmd));
	close(fd);
}

void muteSet()
{
	char mutecmd[CMDSIZE];
	memset(mutecmd,'\0',sizeof(mutecmd));
	strcpy(mutecmd,"mute\n");
	int fd;
	if((fd = open(FIFO,O_WRONLY | O_NONBLOCK)) < 0)
		perror("打开管道错误");
	write(fd,mutecmd,strlen(mutecmd));
	close(fd);
}

void seekFront()
{
	char seekcmd[CMDSIZE];
	memset(seekcmd,'\0',sizeof(seekcmd));
	strcpy(seekcmd,"seek 10\n");
	int fd;
	if((fd = open(FIFO,O_WRONLY | O_NONBLOCK)) < 0)
		perror("打开管道错误");
	write(fd,seekcmd,strlen(seekcmd));
	close(fd);
}

void seekBack()
{
	char seekcmd[CMDSIZE];
	memset(seekcmd,'\0',sizeof(seekcmd));
	strcpy(seekcmd,"seek -10\n");
	int fd;
	if((fd = open(FIFO,O_WRONLY | O_NONBLOCK)) < 0)
		perror("打开管道错误");
	write(fd,seekcmd,strlen(seekcmd));
	close(fd);
}


void msgTrans(char *msg)
{
	char data[1024] = {0};
	char *p = NULL;
	char *ps = NULL;
	strcpy(data, msg);
	
	p = strtok(data,"\n");
	while(p != NULL)
	{
		ps = strstr(p,"=");
		if(strstr(p,"ANS_FILENAME") != NULL)
		{
			printf("正在播放 %s\n",ps+1);
		}
		else if(strstr(p,"ANS_LENGTH") != NULL)
		{
			printf("歌曲长度:%s(s)\n",ps+1);
		}
		else if(strstr(p,"ANS_TIME_POSITION") != NULL)
		{
			printf("播放到:%s(s)\n",ps+1);
		}
		else if(strstr(p,"ANS_META_ARTIST") != NULL)
		{
			printf("歌手:%s\n",ps+1);
		}
		else if(strstr(p,"ANS_PERCENT_POSITION") != NULL)
		{
			printf("播放进度:%s%%\n",ps+1);
		}
		else
			printf("没有数据: %s\n",p);
		
		p = strtok(NULL,"\n");
	}
}

long ms(char origin[9])										//将mm:ss.ms化为毫秒 
{
	long result=0;
	result=atoi(origin)*60+atoi(origin+3);
	return result;
}

void OutputLyrics(long n)					//   输出模块   
{
	List pp;
	for(pp=Head;pp!=NULL;pp=pp->next)
	{
		if(n == pp->timeLine)
		{
			printf("\033[2J\n********************************************************\n");
			printf("命令:\n");
			printf("l>上一首    | n>下一首   |  +>增加音量   | ->减少音量\n");
			printf("d>快进      | a>快退     |  m>静音       | q>退出\n");
			printf("p>暂停      | s>取消暂停");
			printf("\n********************************************************\n");
			printf("%s\n\033[0m",lrcsave);
			printf("\033[1;31m%s\n\033[0m",pp->verse);
			strcpy(lrcsave,pp->verse);
			//sleep(1);
		}
	}
		/*for(int i = 0;i<n;i++)
		{
			for(pp=Head;pp!=NULL;pp=pp->next)
			{
				if(i == pp->timeLine)
					printf("%s\n",pp->verse);
				
			}
		
	

		//printf("%ld >> %s\n",p->timeLine,p->verse);
		}*/
	//flaglrc = -1;
}

void lrclist()
{
	//Song *p = linklist->head;
	//List_head *linklist = (List_head *)arg;
	char name[NAMESIZE] = {0};
	char incmd[20] = {0};
	int zet = 0;
	bzero(name,NAMESIZE);
	strncpy(name,pnow->name,strlen(pnow->name)-4);
	strcat(name,".lrc")	;
	sleep(1);
	printf("%s....\n",name);

	FILE *fp = fopen(name,"r");
   	char line_buf[256];
   	int i;
   	List head = lyric_list_create(fp);
}

List lyric_list_create(FILE *fp)
{
    List p,q,temp,tempMin;
    char line_buf[256];
	//char time_buf[256];
    int i;
			//让linePointer指向第一个歌词串		
	do
	{
		fgets(line_buf,256,fp);
	}
	while(!(line_buf[2]>='0'&&line_buf[2]<='9'));
			//对每一行歌词进行操作		
	do
	{
		if(line_buf[strlen(line_buf)-1]=='\n')
			line_buf[strlen(line_buf)-1]='\0';		
		do
		{
			i=0;
			do
			{
				i++;
			}while(!(line_buf[i-1]==']'&&line_buf[i]!='['));		
			p=(List)malloc(LEN);
			p->timeLine=ms(line_buf+i-9);
			strcpy(p->verse,line_buf+i);
			if(Head==NULL)
				Head=p;
			else
				q->next=p;
			q=p;
			line_buf[i-10]='\0';						//删除最后的括号
			strcat(line_buf,q->verse);				//巧妙地接回去便于继续解析 
		}while(line_buf[0]=='[');					//解析到没有括号为止 
	}while(!(fgets(line_buf,256,fp)==NULL||line_buf[0]!='['));		//到最后一行 
	q->next=NULL;
	fclose(fp);
}

void lrc_list_destroy()
{
	List p = Head;
	List next = NULL;

	while(p != NULL)
	{
		next = p->next;
		p->next = NULL;
		//Head = Head->next;
		free(p);
		p = next;
	}
	Head = NULL;
}

void *getCmdThread(void *cmd)
{	
	List_head *linklist = (List_head *)cmd;
	
	char incmd[20] = {0};
	char name[NAMESIZE] = {0};
	int volume = 100;

	printf("正在播放 %s...\n",pnow->name);	
	printf("\n********************************************************\n");
	printf("命令:\n");
	printf("l>上一首    | n>下一首   |  +>增加音量   | ->减少音量\n");
	printf("d>快进      | a>快退     |  m>静音       | q>退出\n");
	printf("p>暂停      | s>取消暂停");
	printf("\n********************************************************\n");
	while(1)
	{

		bzero(incmd, 20);
		fflush(stdout);
		fgets(incmd, 20,stdin);
		
		if(flagpause == 1)
			flagpause = -1;
		if(strcmp(incmd,"n\n") == 0)
		{
			if(pnow->next == NULL)
				pnow = linklist->head;
			else
				pnow = pnow->next;
			
			bzero(name,NAMESIZE);
			strcat(name,filename);
			strcat(name,pnow->name);
			changeSong(name);
		}
		else if(strcmp(incmd,"l\n") == 0)
		{
			if(pnow->prev == NULL)
				pnow = plast;
			else
				pnow = pnow->prev;

			bzero(name,NAMESIZE);
			strcat(name,filename);
			strcat(name,pnow->name);
			changeSong(name);
		}
		else if(strcmp(incmd,"s\n") == 0)
		{
			nodePrint(linklist);
			flag = 1;
		}
		else if(strcmp(incmd,"+\n") == 0)
		{
			volumeAdd();
			if(volume < 100)
				volume = volume + 2;
			printf("音量：%d\n",volume);
		}
		else if(strcmp(incmd,"-\n") == 0)
		{
			volumeSub();
			if(volume > 0)
				volume = volume - 2;
			printf("音量：%d\n",volume);
		}
		else if(strcmp(incmd,"m\n") == 0)
		{
			muteSet();
		}
		else if(strcmp(incmd,"d\n") == 0)
		{
			seekFront();
		}
		else if(strcmp(incmd,"a\n") == 0)
		{
			seekBack();
		}
		else
		{
			if(strcmp(incmd,"p\n") == 0 || strcmp(incmd,"pause\n") == 0)
			{
				flagpause = -flagpause;
			}
			
			int fd;
			if((fd = open(FIFO,O_WRONLY | O_NONBLOCK)) < 0)
				perror("打开管道错误");
			write(fd,incmd,strlen(incmd));
			close(fd);
			
			if(strcmp(incmd,"q\n") == 0)
			{
				printf("quit the Macrohard Player\n");
				clsthread = 1;
				unlink(FIFO);
				break;
			}	
		}
		
		sleep(1);
		while(flag)
		{
			usleep(100);
		}
	}
}

void *readThread(void *msg)
{
	List_head *linklist = (List_head *)msg;
	
	char info[1024] = {0};
	char *p = NULL;
	char name[NAMESIZE] = {0};
	int ret = 0;
	long n;
	while(1)
	{
		sem_wait(&sem);
		if(flag == 1)
			flag = 2;
		bzero(info, 1024);
		ret = read(fdpp[0],info,1024);
		while(ret > 0)
		{
			if(flag > 1)	
			{
				flag = 3;	
				msgTrans(info);
			}

			if(strstr(p=info, "ANS_TIME_POSITION") != NULL)
			{
				p = strstr(info,"=");
				//printf("%s\n",p+1);
				n = atoi(p+1);
				//printf("%ld\n",n);
				OutputLyrics(n);
			}
			
			if(strstr(info,"ANS_PERCENT_POSITION") != NULL)
			{
				p = strstr(info,"=");
				if(p[1] == '9' && p[2] == '9')
				{
					sleep(4);
					printf("自动播放下一首歌曲\n");
					if(pnow->next == NULL)
						pnow = linklist->head;
					else
						pnow = pnow->next;
			
					bzero(name,NAMESIZE);
					strcat(name,filename);
					strcat(name,pnow->name);
					changeSong(name);	
					sleep(1);
					printf("\n********************************************************\n");
					printf("命令:\n");
					printf("l>上一首    | n>下一首   |  +>增加音量   | ->减少音量\n");
					printf("d>快进      | a>快退     |  m>静音       | q>退出\n");
					printf("p>暂停      | s>取消暂停");
					printf("\n********************************************************\n");
				}
			}
			
			bzero(info,1024);
			ret = read(fdpp[0],info,1024);
			usleep(500*1000);
		}
		
		if(flag == 3)
		{
			flag = 0;
			printf("\n********************************************************\n");
		}
		sem_post(&sem);
		
		while(flagpause == 1)
		{
			sleep(1);
		}
		if(flagpause == -1)
		{

		}
		
		if(clsthread)
			break;
	}
}

void *dataCmdThread(void *cmd)
{
	int fd = 0;
	int ret = 0;
	char info[1024] = {0};
	char str[] = "get_time_pos\nget_percent_pos\n";
	while(1)
	{
		sem_wait(&sem);
		ret = read(fdpp[0],info,1024);
		while(ret > 0)
		{
			ret = read(fdpp[0],info,1024);
		}
		fd = open(FIFO,O_WRONLY|O_NONBLOCK);
		if(fd < 0)
			perror("打开管道错误");
		
		write(fd,str,strlen(str));
		close(fd);
		sem_post(&sem);
		
		sleep(1);
		while(flagpause == 1)
		{
			sleep(1);
		}
		
		if(clsthread)
			break;
	}
	
	return NULL;
}
