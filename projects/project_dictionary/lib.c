/*************************************************************************
    > File Name:lib.c
    > Author: Emrys
    > Created Time: Thur. 26th MAR 2017 15:30:00
 ************************************************************************/

#include "lib.h"

void main_menu_display()/*开始界面*/{
    printf("* * * * * * * * * * * welcome to Emrys's dictionary * * * * * * * * * * *\n");
    printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
    printf("*> Edition Name:0.1.0 beta                                              *\n");
    printf("*> Author: Emrys                                                        *\n");
    printf("*> Created Time: Thur. 26th MAR 2017 15:30:00                           *\n");
    printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
    getchar();
    system("clear");
    printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
    printf("*1----------query                                                       *\n");
    printf("*2----------add or delete or change the library                         *\n");
    printf("*3----------print the lib                                               *\n");
    printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
    printf("please input the number of operatong which you want.\n");
}

static char file_exist;  
  
int ListCount(FILE *fp)/*计算文件中单词数目*/{
    int count=0;  
    char buffer[TARGET_WORD_BUFFER];  
  
    while(fgets(buffer,sizeof(buffer),fp)){  
        if('#'==buffer[0]){
            ++count;  
    }  
  }
    rewind(fp); // 这一步一定要做，使文件指针指向文件头
    return count;
}// 单词的个数即是链表的长度

dict_t CreateList(FILE *fp,int count)/*创建链表*/{
    dict_t head=(dict_t)malloc(sizeof(word_t));
    dict_t p=head;
    char buf[TARGET_WORD_BUFFER];
    char *strword=NULL;
    char *strmean=NULL;
    int mean_count=1;
    int i=0,j=0;
    int word_size,mean_size;

    if(NULL==head){
        perror("head error\n");
        return NULL;
    }
    head->mean_count=0;
    head->next=NULL;

	for(i=0;i<count;i++){
  	    dict_t new=(dict_t)malloc(sizeof(word_t));
  	    if(NULL==new){
      	    perror("file error!\n");
       	 	exit(1);
  	    }
        j=0;
        mean_count=1;
        memset(buf,0,sizeof(buf));
        fgets(buf,sizeof(buf),fp);  
        word_size=strlen(buf);  
        buf[word_size-1] ='\0'; 
	    strword=&buf[1];
        strcpy(new->word,strword);
  
        memset(buf,0,sizeof(buf)); 
        fgets(buf,sizeof(buf),fp);  
        mean_size=strlen(buf);  
        buf[mean_size-1] ='\0';
	    for(j=0;j<strlen(buf);j++){
		    if(buf[j]=='@'){
		        buf[j]='\n';
		        mean_count++;
		    }
	    }

	    strmean=&buf[6]; 
	    strcpy(new->trans,strmean);
	    new->mean_count=mean_count;
	    new->next=NULL;
	    p->next=new;
	    p=new;
	}
    return head;  
}

void SearchList(dict_t head,int count)/*从链表中查找单词*/{
    dict_t pointer;  
    char str[TARGET_WORD_MAX_SIZE];  
    int i;  
  
    while(1){  
        file_exist=0;  
        pointer=head;  
        printf("Please input a word:");  
        scanf("%s",str);  

        if(strcmp(str,"EXIT")==0){
            exit(1);  
        }
  
        while(pointer!=NULL){  
            if((strcmp(pointer->word,str))==0){   
                file_exist=1;  
                printf("%s\n",pointer->trans);  
            }
            pointer=pointer->next;  
        }  
        if(file_exist==0){  // 这里判断了该单词不存在 
            printf("not find!\n"); 
        }  
    }  
}

void AddWordToText()/*往文件中添加单词*/{
    FILE *pf=fopen(TARGET_TEXT_NAME,"a+");
	if(NULL==pf){
        perror("load dictionary_library.txt")
        return 0;
    }

	char word[60]={'\0'};
	char trans[350]={'\0'};
	printf("please input the word you want to add:\n");
	scanf("%s",word);
	printf("please input the translation:\n");
	scanf("%s",trans);
	word[strlen(word)]='\n';
	trans[strlen(trans)]='\n';
	char POUND_KEY = '#';
    char TRANS_WORD[10]={'T','r','a','n','s',':'};
	fputc(POUND_KEY,pf);
	fputs(word,pf);
	fputs(TRANS_WORD,pf);
	fputs(trans,pf);
	fclose(pf);

	printf("add word successfully\n");
}

void DestroyList(dict_t head)/*清空内存*/{  
    dict_t pointer;  
    while(pointer != NULL){  
        pointer = head;  
        head = head->next;  
        free(pointer);  
    }  
}

//testfuctions
void errortest(){
    printf("error test\n"); 
}//错误测试

void PrintList(dict_t head){  
    dict_t pointer;  
    pointer=head;  
  
    while(pointer != NULL){  
        printf("word:%s\nmean_count=%d\ntrans:%s\n", pointer->word, pointer->mean_count,pointer->trans);  
        pointer=pointer->next;  
    }  
}//链表建立输出测试