#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 60000
#define BORDCAST_IP "224.0.1.1"

int main(int argc,char *argv[])
{
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork");
        exit(-1);
    }
    else if(pid == 0)
    {
        char name[512] = "I says:";
	int sock_fd = 0;
	//char buf[1024] = "";
	unsigned short port = PORT;
	sock_fd = socket(AF_INET,SOCK_DGRAM,0);
	if(sock_fd < 0)
	{
		perror("socket");
		close(sock_fd);
		exit(-1);
	}

	char group_ip[16] = BORDCAST_IP;
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(group_ip);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	setsockopt(sock_fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
	struct sockaddr_in dst_addr;
	bzero(&dst_addr,sizeof(dst_addr));
	dst_addr.sin_family = AF_INET;
	dst_addr.sin_port = htons(port);
	dst_addr.sin_addr.s_addr = inet_addr(group_ip);

	while(1)
	{
		//printf("send to %s:",group_ip);
		char send_buf[512] = "";
        fgets(send_buf,sizeof(send_buf),stdin);
		send_buf[strlen(send_buf)-1] = '\0';
		strcat(name,send_buf);
	    sendto(sock_fd,name,strlen(name),0,(struct sockaddr*)&dst_addr,sizeof(dst_addr));
		bzero(&name,sizeof(name));
		strcat(name,"Emrys says:");
		usleep(10000);
	}
		close(sock_fd);
    }
    else
    {
        int sock_fd = 0;
		//char buf[1024] = "";
		unsigned short port = PORT;
		sock_fd = socket(AF_INET,SOCK_DGRAM,0);
		if(sock_fd < 0)
		{
			perror("socket");
			close(sock_fd);
			exit(-1);
		}
    	char group_ip[16] = BORDCAST_IP;
		struct ip_mreq mreq;
		mreq.imr_multiaddr.s_addr = inet_addr(group_ip);
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    	setsockopt(sock_fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));

		struct sockaddr_in src_addr;
    	bzero(&src_addr,sizeof(src_addr));
    	src_addr.sin_family = AF_INET;
    	src_addr.sin_port = htons(port);
    	src_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    	bind(sock_fd,(struct sockaddr*)&src_addr,sizeof(src_addr));
    	while(1)
    	{
        	char recv_buf[512] = "";
        	char src_ip[512] = "";
        	socklen_t src_len = sizeof(src_addr);
        	recvfrom(sock_fd,recv_buf,sizeof(recv_buf),0,(struct sockaddr*)&src_addr,&src_len);
        	inet_ntop(AF_INET,&src_addr.sin_addr.s_addr,src_ip,INET_ADDRSTRLEN);
        	printf("%s:%d %s\n",src_ip,src_addr.sin_port,recv_buf);
    	}
    	close(sock_fd);
    }
    return 0;
}
