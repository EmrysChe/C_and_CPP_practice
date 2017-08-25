/*************************************************************************
    > File Name: main.c
    > Author: Emrys
    > Created Time: Thur. 26th MAR 2017 15:30:00
 ************************************************************************/

#include "lib.h"

int main(){
    FILE *fp=fopen(TARGET_TEXT_NAME,"r+");
    if(NULL==fp){
        perror("Library load");
        exit(1);
    }  
    dict_t head;  
    int count;
    int choice; 
    while(1){
        count=ListCount(fp);   
        printf("count: %d\n",count);

        head=CreateList(fp,count);
        printf("open sucess!\n");
    
        getchar();
        main_menu_display();
        scanf("%d",&choice);
        if(choice==1){
            SearchList(head,count);
        }
        else
            if(choice==2){
                printf("this function is building\n");
                AddWordToText();
                DestroyList(head);
            }
        else
            if(choice==3){
                PrintList(head);
            }
        else{
            printf("error\n");
        }
    }
    return 0;
}