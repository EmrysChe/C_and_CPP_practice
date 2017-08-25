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
#include <pthread.h>

void *I_send(void *arg);
void *I_recv(void *arg);

int main(int argc,char *argv[])
{
	int sock_raw_fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));

	pthread_t tid1;
	pthread_t tid2;
	pthread_create(&tid1,NULL,(void *)I_send,(void *)sock_raw_fd);
	pthread_create(&tid2,NULL,(void *)I_recv,(void *)sock_raw_fd);
	pthread_detach(tid1);
	pthread_detach(tid2);
	while(1);
	
	return 0;
}

void *I_send(void *arg)
{
	int i;
	int sock_raw;
	sock_raw = (int)arg;
	unsigned char send_msg[1024] = {
		0xff,0xff,0xff,0xff,0xff,0xff,
		0x00,0x0c,0x29,0xe5,0x05,0x46,
		0x08,0x06,
		0x00,0x01,0x08,0x00,
		0x06,0x04,0x00,0x01,
		0x00,0x0c,0x29,0xe5,0x05,0x46,
		192,168,31,9,
		0x00,0x00,0x00,0x00,0x00,0x00,
		192,168,31,0
	};
	for(i = 0;i < 255;i++)
	{
		send_msg[41] = i;
		//printf("%d\n",send_msg[41]);
		struct sockaddr_ll sll;
		struct ifreq ethreq;
		strncpy(ethreq.ifr_name,"ens37",IFNAMSIZ);
	
		ioctl(sock_raw,SIOCGIFINDEX,(char*)&ethreq);
		bzero(&sll,sizeof(sll));
		sll.sll_ifindex = ethreq.ifr_ifindex;
		sendto(sock_raw,send_msg,42,0,(struct sockaddr *)&sll,sizeof(sll));
		usleep(1000000);
		if(i == 254)
		{
			i = 0;
		}
	}
}

void *I_recv(void *arg)
{
	while(1)
	{
		int sock_raw;
		sock_raw = (int)arg;
		unsigned char recv_msg[1024] = "";
		recvfrom(sock_raw,recv_msg,sizeof(recv_msg),0,NULL,NULL);
		if(recv_msg[21] == 2)
		{
			char resp_mac[18] = "";
			char resp_ip[16] = "";
	
			sprintf(resp_mac,"%02x%02x:%02x%02x:%02x%02x",recv_msg[22],recv_msg[23],recv_msg[24],recv_msg[25],recv_msg[26],recv_msg[27]);
	
			sprintf(resp_ip,"%03d.%03d.%03d.%03d",recv_msg[28],recv_msg[29],recv_msg[30],recv_msg[31]);
	
			printf("IP:%s - MAC:%s\n",resp_ip,resp_mac);
		}
	}
}