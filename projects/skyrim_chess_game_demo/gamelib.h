#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct player{
    char name[20];
    int sex;
    int age;
    int point;
    int stop_flag;
};

void event(int *point,int *stop_flag)/*事件函数*/{
    if(*point==4){
        printf("安杜因攻击雪漫城，停滞一天\n");
        *stop_flag=1;
    }
    if(*point==5){
        printf("加入战友团，基佬团任务出现bug，停滞一天\n");
        *stop_flag=1;
    }
    if(*point==6){
        printf("到达雪漫城马厩，移动至风舵城外\n");
        *point=9;
    }
    if(*point==10){
        printf("遭遇风暴斗篷士兵（抓根宝:为了帝国！风暴斗篷士兵：天际属于诺德人！），发生缠斗，停滞一天\n");
        *stop_flag=1;
    }
    if(*point==11){
        printf("到达风舵城，前进3格\n");
        *point=14;
    }
    if(*point==13){
        printf("随意杀羊，被风舵城士兵逮捕，拘留一天\n");
        *stop_flag=1;
    }
    if(*point==17){
        printf("到达风舵城马厩，移动至寒冬堡外\n");
        *point=24;
    }
    if(*point==19){
        printf("进入寒冬领，天太冷迷路，滞留一天\n");
        *stop_flag=1;
    }
    if(*point==25){
        printf("到达寒冬堡，前进5格\n");
        *point=30;
    }
    if(*point==29){
        printf("加入法师学院，沉迷矮人地下城搜索，停滞一天\n");
        *stop_flag=1;
    }
    if(*point==34){
        printf("发现《禁忌的传说》,开始寻找高尔多的碎片，移动到格尔蒙德大厅\n");
        *point=60;
        sleep(3);
        printf("\n");
        printf("到达格尔蒙德大厅，停滞三天\n");
        *stop_flag=3;
    }
    if(*point==39){
        printf("遭遇吸血鬼攻城，停滞一天\n");
        *stop_flag=1;
    }
    if(*point==40){
        printf("招募到神烦大小姐，被烦死，停滞一天\n");
        *stop_flag=1;
    }
    if(*point==45){
        printf("成为雪漫男爵，招募到莉迪亚，战斗力大增，前进2格\n");
        *point=47;
    }
    if(*point==57){
        printf("帝国万岁！天际属于帝国\nnothing happened\n");
    }
    if(*point==62){
        printf("学会龙吼：旋风冲刺，前进5格\n");
        *point=67;
    }
    if(*point==68){
        printf("得骨质石化病，停滞两天\n");
        *stop_flag=2;
    }
    if(*point==73){
        printf("获得女士之石的祝福，倒退4格\n");
        *point=69;
    }
    if(*point==80){
        printf("我不想写事件了，好烦，下回合你就赢了，请一定要坚持到下回合，请尽情地嘲笑你的队友吧，位移到99\ncode:point=99\n");
        *point=99;
    }
    if(*point==83){
        printf("天际属于诺德人！停滞一天\n");
        *stop_flag=1;
    }
    if(*point==99){
        printf("恭喜你，这里是第99格\n");
        sleep(3);
        printf("奥杜因把你吃了，再见抓根宝\nno zuo no die why you try\ncode:stop_flag=9999\n");
        *stop_flag=9999;
    }
    if(*point>=100){
        printf("恭喜你，抓根宝，你杀死了傲娇因\n");
        exit(0);
    }
}