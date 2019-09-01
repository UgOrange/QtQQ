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
#include <map>
#include"func.h"
using namespace std;
list<int> clients_list;
#define serverIP "39.107.105.182"
#define serverPort 8888
#define epollSize 1000
#define buffSize 0xFFFF
SqlServer database;

#include <time.h>
map<int,int> userlist;
#define STR_LEN 16//定义随机输出的字符串长度。
char *GenerateStr(char *str)
{
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
    cout<<token1<<" "<<token<<endl;
    string token2=token;
    if(!token1.compare(token2))
    {
        cout<<"123";
        return true;
    }
    else
    {
        cout<<"456";
        return false;
    }
}
void reg(int clientFd,char message[buffSize])
{
    char result[buffSize];
    char uName[1024]={"0"},passWd[1024]={"0"},nickName[1024]={"0"},iconId[1024]={"0"},signature[1024]={"0"},secureId[1024]={"0"},secureAnswer[1024]={"0"};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%s",uName,passWd,nickName,iconId,signature,secureId,secureAnswer);
    cout<<uName<<" "<<passWd<<" "<<nickName<<" "<<iconId<<" "<<signature<<" "<<secureId<<" "<<secureAnswer<<endl;
    ostringstream ostr;
    
    ostr<<"INSERT INTO user (user_name, password, nickname,head_portrait_id, signature,  password_protect_id, answer,VIP) VALUES ('"<<uName<<"','"<<passWd<<"','"<<nickName<<"','"<<iconId<<"','"<<signature<<"','"<<secureId<<"','"<<secureAnswer<<"','0')";
    string sql=ostr.str();
    if(database.query_sql(sql))
    {
        strcpy(result,"register_succ|注册成功！");
    }
    else
    {
        ostringstream ostr1;
        ostr1<<"SELECT * FROM user WHERE user_name = '"<<uName<<"')";
        string sql=ostr1.str();
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
    char uName[1024],passWd[1024];
    char token[1024];
    bzero(result,buffSize);
    bzero(uName,1024);
    bzero(passWd,1024);
    bzero(token,1024);
    sscanf(message,"%[^|]|%s",uName,passWd);
    ostringstream ostr;
    ostr<<"SELECT password FROM user WHERE user_name = '"<<uName<<"'";
    string sql=ostr.str();
    cout<<sql<<endl;
    string ret=database.query(sql);
    if(ret==passWd)
    {
        ostringstream ostr1;
        ostr1<<"SELECT userid FROM user WHERE user_name ='"<<uName<<"'";
        string sql1=ostr1.str();
        string uid=database.query(sql1);
        GenerateStr(token);
        ostringstream ostr2;
        ostr2<<"UPDATE user SET token = '"<<token<<"' , online_status = 1 WHERE userid ='"<<uid<<"'";
        string sql2=ostr2.str();
        bool a=database.query_sql(sql2);
        if(a)
        {
            int i;
            string str11="login_succ|"+uid+"|"+token;
            for( i=0;i<str11.length();i++)
                result[i] = str11[i];
                result[i] = '\0';
            std::stringstream ss;
            ss<<uid;
            int useruid;
            ss>>useruid;
            userlist.insert(map<int, int>::value_type(useruid,clientFd));
            cout<<"User "<<uName<<"is login,uid is "<<useruid<<endl;
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
        ostringstream ostr;
        ostr<<"UPDATE user SET nickname = '"<<nickName<<"', signature = '"<<signature<<"', head_portrait_id = '"<<iconId<<"', password_protect_id = '"<<secureId<<"', answer ='"<<secureAnswer<<"' WHERE userid = '"<<uid<<"'";
        string sql=ostr.str();
        bool a=database.query_sql(sql);
        if(a)
        {
            strcpy(result,"update_profile_succ|修改成功！");
        }
        else{
            strcpy(result,"update_profile_error|修改失败！");
        }
    }
    else{
        strcpy(result,"update_profile_error|token错误，请重新登录！");
    }
    send(clientFd,&result,sizeof(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void LoadFriend(int clientFd,char message[buffSize])
{
    
    char result[buffSize]={0};
    int result1=0;
    char result1_char[buffSize];
    char uid[1024],token[1024];
    MYSQL_ROW row;
    MYSQL mysql;
    MYSQL_RES *result_sql;
    sscanf(message,"%[^|]|%s",uid,token);
    bool a = checktoken(uid,token);
    if(a)
    {
        ostringstream ostr;
        ostr<<"SELECT COUNT(friend_id) FROM friend_info WHERE userid = '"<<uid<<"'";
        string sql1 = ostr.str();
        result1=database.query_sql(sql1);
        //itoa(result1,result1_char,10);
        if(result1_char[0]=='0'){
            strcpy(result,"get_user_friend|您暂时没有好友！");
            return;
        }
        ostringstream ostr1;
        ostr1<<"SELECT friend_id FROM friend_info WHERE userid = '"<<uid<<"'";
        string sql2 = ostr1.str();
        mysql_query(&mysql,sql2.data());
        result_sql = mysql_store_result(&mysql);
        while(row = mysql_fetch_row(result_sql))
        {
            strcat(result1_char,"|");
            strcat(result1_char,row[0]);
        }
        strcpy(result,"get_user_friend|您无权进行此操作！");
    }
    else 
    {
        strcpy(result,result1_char);
    }
    send(clientFd,&result,sizeof(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void sendMessage(int clientFd,char message[buffSize])
{
    char result1[buffSize]={0};
    char result[buffSize]={0};
    char type[1024]={0},uid1[1024]={0},uid2[1024]={0},time[1024]={0},data[1024]={0},token[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%s",type,uid1,uid2,time,data,token);
    bool a =checktoken(uid1,token);
    if(a)
    {
        string strr1="message|";
        string strr2="|";
        string messagestr=strr1+(string)type+strr2+(string)uid1+strr2+(string)time+strr2+(string)data;
        int i=0;
        for( i=0;i<messagestr.length();i++)
            result[i] = messagestr[i];
            result[i] = '\0';
        map<int, int>::iterator iter;
        std::stringstream ss;
            ss<<uid2;
            int useruid;
            ss>>useruid;
            cout<<"UserUid is "<<useruid;
        iter=userlist.find(useruid);
        //TODO:add database
        if(iter!=userlist.end())
        {   
            send(iter->second,&result,sizeof(result),0);
            cout<<"发送给id="<<iter->second<<" data is :"<<result<<endl;
            strcpy(result1,"send_message_succ|发送成功！");
            
        }
        else//对方离线
        {
            cout<<"user is not login!!"<<endl;
        }
    }
    else
    {
        strcpy(result1,"update_profile_error|token错误，请重新登录！");
    }
    send(clientFd,&result1,sizeof(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result1<<endl;
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
        else if(0==strcmp(type,"login"))
        {
            login(clientFd,message);
        }
        else if(0==strcmp(type,"update_profile"))
        {
            updateProfile(clientFd,message);
        }
        else if(0==strcmp(type,"send_message"))
        {
            sendMessage(clientFd,message);
        }
    }
    return 0;
}

#endif 