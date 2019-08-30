#include "epoll.h"
#include <mysql/mysql.h>

int main()
{
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0) { perror("listener"); exit(-1);}
    printf("listen socket created \n");
    //绑定地址
    if( bind(listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind error");
        exit(-1);
    }
    int ret = listen(listener, 5);
    if(ret < 0) { perror("listen error"); exit(-1);}
    printf("Start to listen: %s\n", SERVER_IP);
    int epfd = epoll_create(epollSize);
    if(epfd < 0) { perror("epfd error"); exit(-1);}
    printf("epoll created, epollfd = %d\n", epfd);
    static struct epoll_event events[epollSize];//往内核事件表里添加事件
    addFd(epfd, listener, true);
    while(1)
    {
        int epollEventCount=epoll_wait(epfd,events,epollSize,-1);
        if(epollEventCount<0)
        {
            perror("epoll failure\n");
            break;
        }
        printf("epollEventCount=%d\n",epollEventCount);
        for(int i=0;i<epollEventCount;i++)
        {
            int  socketFd=events[i].data.fd;
            if(socketfd==listener)
            {
                struct sockaddr_in clientAddress;
                socklen_t clientAddrLength=sizeof(struct sockaddr_in);
                int clientfd=accept( listener, ( struct sockaddr* )&client_address, &client_addrLength );
                printf("client connection from: %s : % d(IP : port), clientfd = %d \n",
                 printf("Add new clientfd = %d to epoll\n", clientfd);
                printf("Now there are %d clients int the chat room\n", (int)clients_list.size());

            }
            else
            {
                 int ret = handleRecv(sockfd);
                if(ret < 0) { perror("error");exit(-1); }
            }
        }
    }
    close(listener); //关闭socket
    close(epfd);    //关闭内核
    return 0;
}