# Macrohard_Player
A simple music player base on Mplayer，only working on Linux terminal

## 2017.07.21 creat project
创建了项目。

本项目为Linux环境编程的练习项目，基于Mplayer，由Macrohard小组完成，本次上传的为本人完成部分，由于能力有限，代码参考于CSDN诸多博客。可能日后本人会更新新的功能，时间不确定。

本次上传的版本功能含：读取用户指定文件夹内的音乐文件列表并播放、自动切歌、改变音量(显示的音量为虚假音量而不是实际音量)、静音、快进快退。

本项目可由GCC编译，仅能运行于Linux终端，不具有图形用户界面。

## 2017.07.22 creat Bate_Version

更新了Bate_Version，增加了显示歌词的功能，但由于一些限制，所以取消掉了读取歌曲长度的功能，因为一些令人郁闷的原因(其实是我们不想再改太多东西了)，s命令从原来的显示歌曲信息改成了取消暂停。

对git库进行了一些整理，将原有的文件放到了Origin_Version文件夹中。

本版本存在bug，只能播放"./"目录下的歌曲，原因大概在歌词链表部分，具体还没查出，不知道怎么修改。

歌词功能由Lu Yulong同学制作。

增加的函数为：

1. void OutputLyrics(long n);输出歌词
2. List lyric_list_create(FILE *fp);创建歌词链表
3. long ms(char origin[9]);
4. void lrclist();
5. void lrc_list_destroy();销毁歌词链表

## 2017.07.23 update

优化了歌词界面