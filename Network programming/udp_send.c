#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc,char *argv[])
{
    unsigned short port = 8000;
    char *server_ip = "192.168.87.1";

    if(argc > 1)
    {
        server_ip = argv[1];
    }
    if(argc > 2)
    {
        port = atoi(argv[2]);
    }
    int sockfd;
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0)
    {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in dest_addr;
    bzero(&dest_addr,sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    inet_pton(AF_INET,server_ip,&dest_addr.sin_addr);
    printf("send data to UDP server %s:%d\n",server_ip,port);
    printf("%d\n",dest_addr.sin_addr.s_addr);
    while(1)
    {
        char send_buf[5112] = "aha!";
        //fgets(send_buf,sizeof(send_buf),stdin);
        //send_buf[strlen(send_buf)-1] = '\0';
        sendto(sockfd,send_buf,strlen(send_buf),0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
    }
    close(sockfd);
    return 0;
}
