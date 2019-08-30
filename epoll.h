#ifndef EPOLL_H_INCLUDED
#define EPOLL_H_INCLUDED
#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
list<int> clients_list;
#define serverIP '127.0.0.1'
#define serverPort 8888
#define epollSize 1000
#define buffSize 0xFFFF
int setNonBlock(int sockfd)//设置非阻塞函数模块
{
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)| O_NONBLOCK);
    return 0;
}
void addFd(int epollFd,int fd,bool enable_et)
{
    struct epoll_event ee;
    ee.data.fd=fd;
    ee.events=EPOLLIN;
    if(enable_et)
        ee.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd,EPOLL_CTL_ADD, fd, &ev)
    setNonBlock(fd);
    printf("an fd added to epoll!");
}
int handleRecv(int clientFd)
{
    char bufRecv[buffSize],message[buffSize];
    bzero(buf,buffSize);
    bzero(message,buffSize);
    printf("read from client(clientID = %d)\n", clientFd);
    int len = recv(clientFd, buf, BUF_SIZE, 0);
    if(len == 0)  // len = 0 means the client closed connection
    {
        close(clientfd);
        clients_list.remove(clientfd); //server remove the client
        printf("ClientID = %d closed.\n now there are %d client in the char room\n", clientfd, (int)clients_list.size());
    }
    else
    {
        
    }
}