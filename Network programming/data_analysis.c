#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ether.h>

int main(int argc,char *argv[])
{
	int i = 0;
	unsigned char buf[1024] = "";
	unsigned char type[10] = {1,6,17};// ICMP->1  TCP->6  UDP->17
	char name[10][10] = {"ICMP","TCP","UDP"};
	int sock_raw_fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));

	while(1)
	{
		unsigned char src_mac[18] = "";
		unsigned char dst_mac[18] = "";
		unsigned char dst_ip[16] = "";
		unsigned char src_ip[16] = "";
		recvfrom(sock_raw_fd,buf,sizeof(buf),0,NULL,NULL);
		sprintf(dst_mac,"%02x%02x%02x%02x%02x%02x",
			buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
		sprintf(src_mac,"%02x%02x%02x%02x%02x%02x",
			buf[6],buf[7],buf[8],buf[9],buf[10],buf[11]);

		if(buf[12] == 0x08 && buf[13] == 0x00)
		{
			printf("______IP数据报______\n");
			printf("Src_MAC: %s >> Dst_MAC: %s\n",src_mac,dst_mac);
			//获取源IP，目的IP
			sprintf(src_ip,"%d:%d:%d:%d",buf[26],buf[27],buf[28],buf[29]);
			sprintf(dst_ip,"%d:%d:%d:%d",buf[30],buf[31],buf[32],buf[33]);
			printf("IP:%s >> %s \n",src_ip,dst_ip);
			for(i = 0; i < 10; i++)
			{
				if(buf[23] == type[i])
				{
					printf("协议类别:%s\n",name[i]);
				}
			}
		}
		if(buf[12] == 0x08 && buf[13] == 0x06)
		{
			printf("______ARP数据报______\n");
			printf("Src_MAC: %s >> Dst_MAC: %s\n",src_mac,dst_mac);
			sprintf(src_ip,"%d:%d:%d:%d",buf[28],buf[29],buf[30],buf[31]);
			sprintf(dst_ip,"%d:%d:%d:%d",buf[38],buf[39],buf[40],buf[41]);
			printf("IP:%s >> %s \n",src_ip,dst_ip);
		}
		if(buf[12] == 0x80 && buf[13] == 0x35)
		{
			printf("______RARP数据报______\n");
			printf("Src_MAC: %s >> Dst_MAC: %s\n",src_mac,dst_mac);
			sprintf(src_ip,"%d:%d:%d:%d",buf[28],buf[29],buf[30],buf[31]);
			sprintf(dst_ip,"%d:%d:%d:%d",buf[38],buf[39],buf[40],buf[41]);
			printf("IP:%s >> %s \n",src_ip,dst_ip);
		}
	}
	return 0;
}
