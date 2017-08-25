/*********************************************************************************
  *Copyright(C),Team Macrohard
  *FileName:basic.h
  *Author:Emrys
  *Version:1.170721
  *Date:2017.07.21
  *Description: head file include basic function of music player
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
**********************************************************************************/

#ifndef _BASIC_H_
#define _BASIC_H_

#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<pthread.h>
#include<semaphore.h>

#include"basic.h"

#define FILESIZE 100
#define CMDSIZE 100
#define NAMESIZE 50
#define MAXLINE 4096
#define FIFO "fifo"

char filename[FILESIZE];

struct song
{
	char name[NAMESIZE];
	struct song *next;
	struct song *prev;
};

struct list_head
{
	struct song *head;
	int song_num;
};

typedef struct song Song;
typedef struct list_head List_head;

//创建链表头
List_head *headCreate(void);

//创建结点
Song *nodeCreate(char *name);

//添加结点到链表尾部
void nodeInsertTail(List_head *linklist, Song *pSong);

//打印整张链表
void nodePrint(List_head *linklist);

//查找结点数据，返回结点地址，失败返回NULL
Song *nodeSearch(List_head *linklist, char *name);

//销毁整张链表
void listDestroy(List_head *linklist);

//获得歌曲链表
List_head *getSongList(char *pathname);

//带空格的歌曲名转换格式，空格前加"\"
void blankChange(char *name);

//切换歌曲
void changeSong(char *name);

//增加音量
void volumeAdd();

//减少音量
void volumeSub();

//静音
void muteSet();

//快退
void seekBack();

//快进
void seekFront();

//处理线程数据
void msgTrans(char *msg);

//获取按键输入命令
void *getCmdThread(void *cmd);

//读取数据并处理
void *readThread(void *msg);

//定时发送命令获取数据
void *dataCmdThread(void *cmd);

#endif

