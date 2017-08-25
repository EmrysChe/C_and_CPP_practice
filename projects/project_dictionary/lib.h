/*************************************************************************
    > File Name:lib.h
    > Author: Emrys
    > Created Time: Thur. 26th MAR 2017 15:30:00
 ************************************************************************/

#ifndef _TARGET_H_
#define _TARGET_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h> 

#define TARGET_TEXT_NAME            "./dictionary_library.txt" 

#define TARGET_WORD_MAX_SIZE        64
#define TARGET_WORD_MEANING_COUNT   32
#define TARGET_WORD_MAX_TRANSLATION 512
#define TARGET_WORD_BUFFER          1024

// 单词链表 ---- 单词名称，单词有几种翻译，单词翻译的结果  
typedef struct dict{  
  char word[TARGET_WORD_MAX_SIZE]; // 要输入的单词，如"#a"  
  int mean_count;  // 单词解释的个数，如既可以做名词也可以做动词，用@分开  
  char trans[TARGET_WORD_MAX_TRANSLATION]; // 翻译结果  
  struct dict *next;  
}word_t,*dict_t; 

//finished
void main_menu_display();/*开始界面*/
int ListCount(FILE *fp);/*计算文件中单词数目*/
dict_t CreateList(FILE *fp,int count);/*创建链表*/
void SearchList(dict_t head,int count);/*从链表中查找单词*/
void DestroyList(dict_t head);/*清空内存*/
void AddWordToText();/*往文件中添加单词*/

//test functions
void PrintList(dict_t head);
void errortest();

#endif