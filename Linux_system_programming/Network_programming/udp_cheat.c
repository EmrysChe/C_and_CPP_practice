#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>

unsigned short checksum(unsigned short *buf, int nword)
{
	unsigned long sum;
	for(sum = 0;nword > 0;nword--)
	{
		sum += htons(*buf);
		buf++;
	}
	sum = (sum>>16) + (sum&0xffff);
	sum += (sum>>16);
	return ~sum;
}

int main(int argc,char *argv[])
{
	int i = 0;
	int sock_raw_fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	struct sockaddr_ll sll;
	struct ifreq ethreq;

	strncpy(ethreq.ifr_name,"ens37",IFNAMSIZ);
	if(-1 == ioctl(sock_raw_fd,SIOCGIFINDEX,&ethreq))
	{
		perror("ioctl");
		close(sock_raw_fd);
		exit(-1);
	}
	bzero(&sll,sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;
	while(1)
	{
		if(i == 255)
		{
			i = 0;
		}
		unsigned char send_msg[1024] = {
			0x9c,0xd2,0x1e,0xfe,0x0d,0xd3,
			0x00,0x0c,0x29,0xe5,0x05,0x46,
			0x08,0x00,
			0x45,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,
			0x80,17,0x00,0x00,
			192,168,0,255,
			192,168,31,26,
			0x09,0x79,0x09,0x79,// 发送端端口号:2425,接收端端口号:2425
			0x00,0x00,0x00,0x00,
		};
		send_msg[33] = i;
		int len = sprintf(send_msg+42,"aha!");
		if(len % 2 == 1)
		{
			len++;
		}
		*((unsigned short *)&send_msg[16]) = htons(20+8+len);
		*((unsigned short *)&send_msg[14+20+4]) = htons(8+len);
	
		unsigned char pseudo_head[1024] = {
			192,168,0,255,
			192,168,31,26,
			0x00,17,0x00,0x00,
		};
		pseudo_head[7] = i;
		i++;
	
		*((unsigned short *)&pseudo_head[10]) = htons(8 + len);
		memcpy(pseudo_head+12, send_msg+34, 8+len);
		*((unsigned short *)&send_msg[24]) = htons(checksum((unsigned short *)(send_msg+14),20/2));
		*((unsigned short *)&send_msg[40]) = htons(checksum((unsigned short *)pseudo_head,(12+8+len)/2));
	
		len = sendto(sock_raw_fd, send_msg, 14+20+8+len, 0 , (struct sockaddr *)&sll, sizeof(sll));
		if(len == -1)
		{
			perror("sendto");
		}
		//usleep(100000);
	}

	return 0;
}