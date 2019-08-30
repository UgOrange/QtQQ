#ifndef EPOLL_H_INCLUDED
#define EPOLL_H_INCLUDED
#include <iostream>
#include <sstream>
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
#define serverPort 8892
#define epollSize 1000
#define buffSize 0xFFFF
SqlServer database;

#include <time.h>
 
#define STR_LEN 16//定义随机输出的字符串长度。
char *GenerateStr()
{
    char str[STR_LEN + 1] = {0};
    int i,flag;
     
    srand(time(NULL));//通过时间函数设置随机数种子，使得每次运行结果随机。
    for(i = 0; i < STR_LEN; i ++)
    {
		flag = rand()%3;
		switch(flag)
		{
		case 0:
			str[i] = rand()%26 + 'a'; 
			break;
		case 1:
			str[i] = rand()%26 + 'A'; 
			break;
		case 2:
			str[i] = rand()%10 + '0'; 
			break;
		}
    }
    printf("%s\n", str);//输出生成的随机数。
     
    return str;
}
bool checktoken(char uid[1024],char token[1024])
{
    ostringstream ostr;
    ostr<<"SELECT token FROM user WHERE userid ='"<<uid<<"'";
    string sql =ostr.str();
    string token1=database.query(sql);
    if(token==token1)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void reg(int clientFd,char message[buffSize])
{
    char result[buffSize];
    char uName[1024]={"0"},passWd[1024]={"0"},nickName[1024]={"0"},iconId[1024]={"0"},signature[1024]={"0"},secureId[1024]={"0"},secureAnswer[1024]={"0"};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%s",uName,passWd,nickName,iconId,signature,secureId,secureAnswer);
    cout<<uName<<" "<<passWd<<" "<<nickName<<" "<<iconId<<" "<<signature<<" "<<secureId<<" "<<secureAnswer<<endl;
    string sql=sprintf("INSERT INTO user (user_name, password, nickname, signature, head_portrait_id, password_protect_id, answer) VALUES ('%s','%s','%s','%s','%s','%s,'%s')",uName,passWd,nickName,iconId,signature,secureId,secureAnswer);
    if(database.query_str(sql))
    {
        strcpy(result,"register_succ|注册成功！");
    }
    else
    {
        string sql=sprintf("SELECT * FROM user WHERE user_name = '%s'",uName);
        string ret=database.query(sql);
        if(ret!="NULL")
        strcpy(result,"register_error|用户名被占用！");
        else
        strcpy(result,"register_error|注册失败！");
    }
    send(clientFd,&result,sizeof(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void login(int clientFd,char message[buffSize])
{
    char result[buffSize];
    char uName[1024],passWd[1024],token[1024];
    bzero(result,buffSize);
    bzero(uName,1024);
    bzero(passWd,1024);
    bzero(token,1024);
    sscanf(message,"%[^|]|%s",uName,passWd);
    string sql=sprintf("SELECT password FROM user WHERE user_name = '%s'",uName);
    string ret=database.query(sql);
    if(ret==passWd)
    {
        string sql1=sprintf("SELECT userid FROM user WHERE user_name ='%s'",uName);
        string uid=database.query(sql1);
        token=GenerateStr();
        string sql2=sprintf("UPDATE user SET token = '%s' , online_status = 1 WHERE userid ='%s'",token,uid);
        bool a=database.query_str(str2);
        if(a)
        {
            result=sprintf("login_succ|%s|%s",uid,token);
        }
        else{
             strcpy(result,"login_error|登录失败！");
        }
    }
    else{
        strcpy(result,"login_error|用户名或密码错误！");
    }
    send(clientFd,&result,sizeof(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void updateProfile(int clientFd,char message[buffSize])
{
    char result[buffSize];
    char uid[1024]={"0"},token[1024]={"0"},nickName[1024]={"0"},iconId[1024]={"0"},signature[1024]={"0"},secureId[1024]={"0"},secureAnswer[1024]={"0"};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%s",uid,token,nickName,iconId,signature,secureId,secureAnswer);
    bool a =checktoken(uid,token);
    if(a)
    {

    }
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