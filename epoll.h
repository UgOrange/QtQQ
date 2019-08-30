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
#include"func.h"
using namespace std;
list<int> clients_list;
#define serverIP "10.194.144.33"
#define serverPort 8889
#define epollSize 1000
#define buffSize 0xFFFF
SqlServer database;
void reg(int clientFd,char message[buffSize])
{
    string uName,passWd,nickName,iconId,signature,secureId,secureAnswer;
    sscanf(message,"%s|%s|%s|%s|%s|%s|%s",uName,passWd,nickName,iconId,signature,secureId,secureAnswer);
    cout<<uName<<passWd<<nickName<<iconId<<signature<<secureId<<secureAnswer<<endl;
}
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
    epoll_ctl(epollFd,EPOLL_CTL_ADD, fd, &ee);
    setNonBlock(fd);
    printf("an fd added to epoll!");
}
int handleRecv(int clientFd)
{
    char bufRecv[buffSize],message[buffSize];
    bzero(bufRecv,buffSize);
    bzero(message,buffSize);
    char type[20]={0};
    printf("read from client(clientID = %d)\n", clientFd);
    int len = recv(clientFd, bufRecv, buffSize, 0);
    if(len == 0)  // len = 0 means the client closed connection
    {
        close(clientFd);
        clients_list.remove(clientFd); //server remove the client
        printf("ClientID = %d closed.\n now there are %d client in the char room\n", clientFd, (int)clients_list.size());
    }
    else
    {
        int i;
        printf("message is %s\n",bufRecv);
        for(i=0;i<strlen(bufRecv);i++)
        {
        //从客户端发来的字符串中取出消息类型
            if(bufRecv[i] == '|') break;
            type[i] = bufRecv[i];
        }
        strcpy(message,bufRecv+i+1);
        cout<<"type is "<<type<<" message is "<<message<<endl;
        if(0==strcmp(type,"register"))
        {
            reg(clientFd,message);
        }
    }
    return 0;
}

#endif 