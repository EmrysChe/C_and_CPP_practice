#include "gamelib.h"

struct player players[5];

void login(int n)/*加入玩家信息*/{
    
    int i;
   
    for(i=0;i<n;i++){
        printf("请输入%d位玩家资料\n",i+1);
        printf("姓名:");
        scanf("%s",players[i].name);
        printf("性别（1---male,2---female）:");
        scanf("%d",&players[i].sex);
        printf("年龄:");
        scanf("%d",&players[i].age);
        players[i].stop_flag=0;
        players[i].point=0;
    }
}

void check(int n)/*打印玩家信息*/{
    
    int i;

    for(i=0;i<n;i++){
        printf("第%d位玩家资料:\n姓名:%s\n性别（1---男性，2---女性）:%d\n年龄:%d\n"
        ,i+1,players[i].name,players[i].sex,players[i].age);
    }
}

void game(int n)/*游戏进程*/{
    
    int c;
    int j;
    int di;
    
    srand((unsigned)time(NULL));

    while(1){
        for(j=0;j<n;j++){
            if(players[j].stop_flag==0){
                printf("%s现在在第%d格，请输入1摇骰子\n",players[j].name,players[j].point);
		        scanf("%d",&c);
                if(c==1){
                    di=(int)(rand()%6)+1;
                    printf("%s的点数为:%d\n",players[j].name,di);
                    printf("如果想直接退出游戏请按Ctrl+c\n");
                    players[j].point+=di;
                    event(&players[j].point,&players[j].stop_flag);
                    printf("%s现在在第%d格\n\n",players[j].name,players[j].point);
                }
                else
                    printf("请输入1摇骰子！\n");
                }
            else{
                players[j].stop_flag=players[j].stop_flag-1;
            }
        }
    }
}

int main()/*main*/{
    
    int amount;

    printf("------------欢迎来到天际省------------\n");
    printf("版本号:for Linux,demo\n作者:Emrys\n");
    printf("请输入玩家数量:");
    scanf("%d",&amount);
    login(amount);
    check(amount);
    game(amount);

    return 0;
}
