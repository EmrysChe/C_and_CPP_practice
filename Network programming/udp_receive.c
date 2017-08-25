#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc ,char *argv[])
{
    unsigned int short port = 8080;
    if(argc > 1)
    {
        port = atoi(argv[1]);
    }
    int sockfd;
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0)
    {
        perror("socket");
        exit(-1);

    }
    struct sockaddr_in my_addr;
    bzero(&my_addr,sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("Bind serber to port %d\n",port);

    int err_log;
    err_log = bind(sockfd,(struct sockaddr*)&my_addr,sizeof(my_addr));
    if(err_log != 0)
    {
        perror("bind");
        close(sockfd);
        exit(-1);
    }

    while(1)
    {
        int recv_len;
        char recv_buf[512] = "";
        struct sockaddr_in client_addr;
        char cli_ip[INET_ADDRSTRLEN] = "";
        socklen_t cliaddr_len = sizeof(client_addr);
        recv_len = recvfrom(sockfd,recv_buf,sizeof(recv_buf),0,(struct sockaddr*)&client_addr,&cliaddr_len);
        inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,cli_ip,INET_ADDRSTRLEN);
        printf("\nip:%s , port:%d \n",cli_ip,ntohs(client_addr.sin_port));
        printf("data(%d):%s\n",recv_len,recv_buf);
    }
    close(sockfd);
    return 0;
}
