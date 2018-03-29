#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>
#include <fcntl.h>

void *client_process(void *arg)
{
    int connfd = (int)arg;
    int fd = 0;
    int len = 0;
    char recv_buf[1024] = "";
    char file_buf[1024] = "";
    char filename[50] = "html/";

    char head[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    
    recv(connfd,recv_buf,sizeof(recv_buf),0);

    sscanf(recv_buf,"GET /%[^ ]",(filename + 5));
    if(filename[5] == 0)
    {
        strcpy(filename,"html/index.html");
    }
    printf("filename = *%s*\n",filename);
    fd = open(filename,O_RDONLY);
    if(fd < 0)
    {
        char err[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<HTML><BODY>File Not Found</BODY></HTML>";
        perror("open");
        send(connfd,err,strlen(err),0);
        close(connfd);
        return NULL;
    }

    send(connfd,head,strlen(head),0);
    printf("send head\n");
    while((len = read(fd,file_buf,sizeof(file_buf))) > 0)
    {
        send(connfd,file_buf,len,0);
    }
    close(fd);
    close(connfd);
    return NULL;
}

int main(int argc,char *argv[])
{
    unsigned short port = 8000;
	if(argc > 1)
	{
		port = atoi(argv[1]);
	}
    printf("Listenning at port :%d\n",port);
    printf("Usage: http/192.168.31.7:%d\n",port);

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
    {
        perror("socket");
        exit(0);
    }

    struct sockaddr_in my_addr;
    bzero(&my_addr,sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    int err_log = bind(sockfd,(struct sockaddr*)&my_addr,sizeof(my_addr));
    if(err_log != 0)
    {
        perror("bind");
        close(sockfd);
        exit(-1);
    }
    err_log = listen(sockfd,10);
    if(err_log != 0)
    {
        perror("listen");
        close(sockfd);
        exit(-1);
    }
    while(1)
    {
        char cli_ip[INET_ADDRSTRLEN] = "";
        struct sockaddr_in client_addr;
        socklen_t cliaddr_len = sizeof(client_addr);

        int connfd = accept(sockfd,(struct sockaddr*)&client_addr,&cliaddr_len);
        if(connfd < 0)
        {
            perror("accept");
            continue;
        }
        else
        {
            inet_ntop(AF_INET,&client_addr.sin_addr,cli_ip,INET_ADDRSTRLEN);
            printf("connected from %s\n\r",cli_ip);
            pthread_t tid;
            pthread_create(&tid,NULL,client_process,(void*)connfd);
            pthread_detach(tid);
        }
    }
    close(sockfd);
    return 0;
}
