#ifndef __FUNC_H__
#define __FUNC_H__
#include "sql.h"
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
#define buffSize 0xFFFFF
#define STR_LEN 16//定义随机输出的字符串长度。
SqlServer database;
using namespace std;
class ServerFunc{
      private:
            map<int,int> userlist;
            int count;
            map<int, int>::iterator iter;
      public:
            ServerFunc();
            char *GenerateStr(char *str);
            bool checktoken(char uid[1024],char token[1024]);
            void reg(int clientFd,char message[buffSize]);
            void login(int clientFd,char message[buffSize]);
            void logout(int clientFd);
            void updateProfile(int clientFd,char message[buffSize]);
            void LoadFriend(int clientFd,char message[buffSize]);
            void sendMessage(int clientFd,char message[buffSize]);
            void forgetPassWd(int clientFd,char message[buffSize]);
            void getUserInfo(int clientFd,char message[buffSize]);
            void getUserGroup(int clientFd,char message[buffSize]);
            void getGroupInfo(int clientFd,char message[buffSize]);
            void addFriend(int clientFd,char message[buffSize]);
            void delFriend(int clientFd,char message[buffSize]);
            void confirmFriendRequest(int clientFd,char message[buffSize]);
            void createGroup(int clientFd,char message[buffSize]);
            void joinGroup(int clientFd,char message[buffSize]);
            void handleGroupRequest(int clientFd,char message[buffSize]);
            void getUnreadFriendRequest(int clientFd,char message[buffSize]);
            void getUnreadGroupRequest(int clientFd,char message[buffSize]);
            void getUnreadMessage(int clientFd,char message[buffSize]);
            void sendFile(int clientFd,char message[buffSize]);

};

ServerFunc::ServerFunc()
{
    count=0;
}
char *ServerFunc::GenerateStr(char *str)
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
bool ServerFunc::checktoken(char uid[1024],char token[1024])
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
void ServerFunc::reg(int clientFd,char message[buffSize])
{
    char result[buffSize];
    char uName[1024]={0},passWd[1024]={0},nickName[1024]={0},iconId[1024]={0},signature[1024]={0},secureId[1024]={0},secureAnswer[1024]={0};
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
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void ServerFunc::login(int clientFd,char message[buffSize])
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
            ostr.str("");
        ostr<<"SELECT COUNT(poster_id) FROM temperary_message,user  WHERE temperary_message.poster_id = user.userid AND recv_id ='"<<uid<<"' ORDER BY temperary_message.time DESC";
        string sqll=ostr.str();
        ostr.str("");
        string countp2p=database.query(sqll);
        int unreadCount=stoi(countp2p);
        ostr<<"SELECT COUNT(group_chat_info.group_chat_info_id) FROM temperary_message ,user, group_chat_info WHERE temperary_message.poster_id = user.userid AND group_chat_info.group_chat_info_id = temperary_message.group_chat_info_id AND recv_id = '"<<uid<<"' ORDER BY temperary_message.time DESC";
        sqll=ostr.str();
        ostr.str("");
        countp2p=database.query(sqll);
        unreadCount=stoi(countp2p);
            int i;
            string str11="login_succ|"+(string)"登录成功!|"+uid+"|"+token+"|"+to_string(unreadCount);
            for( i=0;i<str11.length();i++)
                result[i] = str11[i];
                result[i] = '\0';
                string ret=database.query(sql);
            std::stringstream ss;
            ss<<uid;
            int useruid;
            ss>>useruid;
            userlist[useruid]=clientFd;
            cout<<"User "<<uName<<"is login,uid is "<<useruid<<endl;
        }
        else{
             strcpy(result,"login_error|登录失败！");
        }
    }
    else{
        strcpy(result,"login_error|用户名或密码错误！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
    iter = userlist.begin();
    while(iter != userlist.end()){
        cout << iter->first << "-" << iter->second << endl;
        iter++;
    }
}
void ServerFunc::logout(int clientFd)
{
map<int,int>::iterator na;
for (na = userlist.begin(); na != userlist.end(); na++)
    {if(na->second==clientFd)
        break;}
    if (na==userlist.end())
        return;
    userlist.erase(na);
    string userid=to_string(na->first);
    string sql="UPDATE user SET token = NULL,online_status = 0 WHERE userid = '";
    sql=sql+userid;
    string str1="'";
    sql=sql+str1;
    database.query_sql(sql);
}
void ServerFunc::updateProfile(int clientFd,char message[buffSize])
{
    char result[buffSize];
    char uid[1024]={0},token[1024]={0},nickName[1024]={0},iconId[1024]={0},signature[1024]={0},secureId[1024]={0},secureAnswer[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%[^|]|%s",uid,token,nickName,iconId,signature);
    bool a =checktoken(uid,token);
    if(a)
    {
        ostringstream ostr;
        ostr<<"UPDATE user SET nickname = '"<<nickName<<"', signature = '"<<signature<<"', head_portrait_id = '"<<iconId<<"'"<<" WHERE userid = '"<<uid<<"'";
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
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void ServerFunc::LoadFriend(int clientFd,char message[buffSize])
{
    
    char result[buffSize]={0};
    string result1;
    char result1_char[buffSize]={0};
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
        result1=database.query(sql1);
        stringstream ss;
        ss<<result1;
        int friendcount;
        ss>>friendcount;
        if(friendcount==0){
            strcpy(result,"get_user_friend|您暂时没有好友！");
        }
        ostringstream ostr1;
        ostr1<<"SELECT friend_id FROM friend_info WHERE userid = '"<<uid<<"'";
        string sql2 = ostr1.str();
        strcpy(result,"friendinfo|");
        strcat(result,result1.c_str());
        for(int i=0;i<friendcount;i++)
        {
            strcat(result,"|00");
            string uidd=database.query(sql2,i);
            strcat(result,uidd.c_str());
        }
    }
    else 
    {
        strcpy(result,"get_user_friend|您无权进行此操作！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void ServerFunc::sendMessage(int clientFd,char message[buffSize])
{
    char result1[buffSize]={0};
    char result[buffSize]={0};
    char type[1024]={0},uid1[1024]={0},uid2[1024]={0},time[1024]={0},data[1024]={0},token[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%s",type,uid1,uid2,time,data,token);
    bool a =checktoken(uid1,token);
    if(a)
    {
        if(type[0]=='0'||type[0]=='2')
        {
            string strr1="message|";
            string strr2="|";
            string messagestr=strr1+(string)type+strr2+(string)uid1+strr2+(string)time+strr2+(string)data;
            int i=0;
            for( i=0;i<messagestr.length();i++)
                result[i] = messagestr[i];
                result[i] = '\0';
            std::stringstream ss;
            ss<<uid2;
            int useruid;
            ss>>useruid;
            cout<<"UserUid is "<<useruid<<endl;
        iter=userlist.find(useruid);
            ostringstream ostr;
            ostr<<"SELECT single_chat_info_id FROM single_chat_info WHERE (member_id1 ='"<<uid1<<"' AND member_id2 = '"<<uid2<<"') OR (member_id1 = '"<<uid2<<"' AND member_id2 = '"<<uid1<<"')";
            string sql=ostr.str();
            string single_chat_info_id=database.query(sql);
            ostr.str("");
            ostr<<"INSERT INTO single_chat_history (chat_info_id,poster_id,time,content) VALUES ('"<<single_chat_info_id<<"','"<<uid1<<"','"<<time<<"','"<<data<<"')";
            sql=ostr.str();
            bool a=database.query_sql(sql);
        //TODO:add database
        if(iter!=userlist.end())
        {   

            send(iter->second,&result,strlen(result),0);
            cout<<"发送给id="<<iter->second<<" data is :"<<result<<endl;
            strcpy(result1,"send_message_succ|发送成功！");
            bzero(result,sizeof(result));
        }
        else//对方离线=database.query_sql(
        {
            ostr.str("");
            ostr<<"INSERT INTO temperary_message (poster_id,recv_id,time,content) VALUES ('"<<uid1<<"','"<<uid2<<"',"<<"','"<<time<<"','"<<data<<"')";
            sql=ostr.str();
            database.query_sql(sql);
            cout<<"user is not login!!"<<endl;
            strcpy(result1,"send_message_succ|对方离线！");
        }

        }
        if(type[0]=='1'||type[0]=='3')
        {
            ostringstream ostr;
        ostr.str("");
        ostr<<"SELECT COUNT(member_id) FROM group_chat_info WHERE group_chat_info_id = '"<<uid2<<"'";
        string sql=ostr.str();
        string count1=database.query(sql);
        ostr.str("");
        stringstream ss;
        ss<<count1;
        int mCount;
        ss>>mCount;
        ostr.str("");
        ostr<<"SELECT member_id FROM group_chat_info WHERE group_chat_info_id ='"<<uid2<<"'";
        sql=ostr.str();
        int uidtest;
        ostringstream ostr1;
        ostr1<<" INSERT INTO group_chat_history (group_chat_info_id,time,poster_id,content) VALUES ('"<<uid2<<"','"<<time<<"','"<<uid1<<"','"<<data<<"');";
        string sql1=ostr1.str();
        database.query_sql(sql1);
        for (int i=0;i<mCount-1;i++)
        {
            ss.str("");
            string uidd=database.query(sql,i);
            ss<<count1;
            ss>>uidtest;
            cout<<"UserUid is "<<uidtest<<endl;

            iter=userlist.find(uidtest);
            if(iter!=userlist.end())
            {
                strcpy(result,"message|");
                strcat(result,type);
                strcat(result,"|");
                strcat(result,uid1);
                strcat(result,"|");
                strcat(result,time);
                strcat(result,"|");
                strcat(result,data);
                send(iter->second,&result,strlen(result),0);
                if(iter->second==clientFd)
                {
                }
                else
                {
                    cout<<"发送给id="<<iter->second<<" data is :"<<result<<endl;
                }
                strcpy(result1,"send_message_succ|发送成功！");
               
            }
            else
            {
                ostringstream ostr2;
                ostr1<<"  INSERT INTO temperary_message(poster_id,recv_id,time,content) VALUES ('"<<uid1<<"','"<<time<<"','"<<uidd<<"','"<<data<<"');";
                string sql1=ostr1.str();
                database.query_sql(sql1);
                strcpy(result1,"send_message_succ|发送成功！");
            } 
        }
        }
    }
    else
    {
        strcpy(result1,"update_profile_error|token错误，请重新登录！");
    }
    send(clientFd,&result1,strlen(result1),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result1<<endl;
}
void ServerFunc::forgetPassWd(int clientFd,char message[buffSize])
{
    char result[buffSize]={0};
    char secureId[1024]={0},secureAnswer[1024]={0},uName[1024]={0},passWd[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%s",uName,secureId,secureAnswer,passWd);
    ostringstream ostr;
    ostr<<"SELECT answer FROM user WHERE user_name = '"<<uName<<"'";
    string sql=ostr.str();
    string answer=database.query(sql);
    ostr.str("");
    ostr<<"SELECT password_protect_id FROM user WHERE user_name = '"<<uName<<"'";
    sql=ostr.str();
    string sid=database.query(sql);
    ostr.str("");
    std::stringstream ss;
            ss<<sid;
            int useruid;
            ss>>useruid;
          std::stringstream ss1;
            ss1<<secureId;
            int useruid1;
            ss>>useruid1;      
    cout<<sid<<" "<<answer<<endl;
    if(useruid==useruid1&&answer==secureAnswer)
    {
        ostr<<"UPDATE user SET password ='"<<passWd<<"' WHERE user_name = '"<<uName<<"'";
        sql=ostr.str();
        database.query_sql(sql);
        strcpy(result,"up_pass_succ|修改成功！");
    }
    else{
        strcpy(result,"up_pass_error|密保错误！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void ServerFunc::getUserInfo(int clientFd,char message[buffSize])
{
    char result[buffSize]={0};
    char uid[1024]={0},token[1024]={0},uid1[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%s",uid,uid1,token);
    bool a =checktoken(uid,token);
    if(a)
    {ostringstream ostr;
    ostr<<"SELECT nickname FROM user WHERE userid = '"<<uid1<<"'";
    string sql=ostr.str();
    string nickname=database.query(sql);
    ostr.str("");
    ostr<<"SELECT head_portrait_id FROM user WHERE userid = '"<<uid1<<"'";
    sql=ostr.str();
    string hid=database.query(sql);
    ostr.str("");
    ostr<<"SELECT signature FROM user WHERE userid = '"<<uid1<<"'";
    sql=ostr.str();
    string sig=database.query(sql);
    ostr.str("");
    if(!strcmp(uid,uid1))
    strcpy(result,"userinfo|");
    else
    {
        strcpy(result,"friend_detail_info|");
    }
    
    strcat(result,nickname.c_str());
    strcat(result,"|");
    strcat(result,hid.c_str());
    strcat(result,"|");
    strcat(result,sig.c_str());}
    else{
        strcpy(result,"update_profile_error|token错误，请重新登录！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void ServerFunc::getUserGroup(int clientFd,char message[buffSize])
{
    char result[buffSize]={0};
    char uid[1024]={0},token[1024]={0};
    sscanf(message,"%[^|]|%s",uid,token);
    bool a =checktoken(uid,token);
    if(a)
    {
        ostringstream ostr;
        ostr<<"SELECT COUNT(group_chat_info_id) FROM group_chat_info WHERE member_id = '"<<uid<<"'";
        string sql=ostr.str();
        ostr.str("");
        string groupCount=database.query(sql);
        stringstream ss;
        ss<<groupCount;
        int gCount;
        ss>>gCount;
        ostr<<"SELECT group_chat_info_id FROM group_chat_info WHERE member_id = '"<<uid<<"'";
        sql=ostr.str();
        strcpy(result,"groupinfo|");
        strcat(result,groupCount.c_str());
        string groupid;
        for (int i=0;i<gCount;i++)
        {
            strcat(result,"|");
            groupid=database.query(sql,i);
            cout<<groupid<<endl;
            strcat(result,groupid.c_str());
        }
    }
    else{
        strcpy(result,"token_error|token错误，请重新登录！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void ServerFunc::getGroupInfo(int clientFd,char message[buffSize])
{
    char result[buffSize]={0};
    char uid[1024]={0},token[1024]={0},groupid[1024];
    sscanf(message,"%[^|]|%[^|]|%s",uid,groupid,token);
    bool a =checktoken(uid,token);
    if(a)
    {
        ostringstream ostr;
        ostr<<"SELECT group_chat_name FROM group_chat_info WHERE group_chat_info_id = '"<<groupid<<"'";
        string sql=ostr.str();
        string groupName=database.query(sql);
        if(groupName=="NULL")
        {
            strcpy(result,"update_profile_error|群id不存在");
        }
        else
        {ostr.str("");
        ostr<<"SELECT COUNT(member_id) FROM group_chat_info WHERE group_chat_info_id = '"<<groupid<<"'";
        sql=ostr.str();
        string count1=database.query(sql);
        ostr.str("");
        stringstream ss;
        ss<<count1;
        int mCount;
        ss>>mCount;
        strcpy(result,"group_detail_info|");
        strcat(result,groupid);
        strcat(result,"|");
        strcat(result,groupName.c_str());
        strcat(result,count1.c_str());
        ostr<<"SELECT member_id FROM group_chat_info WHERE group_chat_info_id ='"<<groupid<<"' AND group_chat_admin = 1";
        sql=ostr.str();
        string adminUid=database.query(sql);
        ostr.str("");
        ostr<<"SELECT member_id FROM group_chat_info WHERE group_chat_info_id ='"<<groupid<<"' AND group_chat_admin = 0";
        sql=ostr.str();
        strcat(result,"|");
        strcat(result,adminUid.c_str());
        for (int i=0;i<mCount-1;i++)
        {
            strcat(result,"|");
            string uidd=database.query(sql,i);
            strcat(result,uidd.c_str());
        }
        }
    }
    else{
        strcpy(result,"token_error|token错误，请重新登录！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void ServerFunc::addFriend(int clientFd,char message[buffSize])
{
    char result[buffSize]={0};
    char uid[1024]={0},token[1024]={0},uid1[1024]={0},request[1024]={0},result1[1024]={0};
    cout<<uid<<" "<<uid1<<" "<<request<<" "<<token<<endl;
    sscanf(message,"%[^|]|%[^|]|%[^|]|%s",uid,uid1,request,token);
    bool a =checktoken(uid,token);
    if(a)
    {
        ostringstream ostr;
        ostr<<"SELECT * FROM friend_info WHERE (userid = '"<<uid<<"' AND friend_id = '"<<uid1<<"') OR (userid = '"<<uid1<<"' AND friend_id = '"<<uid<<"')";
        string sql=ostr.str();
        string ret1=database.query(sql);
        ostr.str("");
        if(ret1=="NULL")
        {ostr<<"INSERT INTO friend_apply (applicant_id,object_id,friend_apply_message) VALUES('"<<uid<<"','"<<uid1<<"','"<<request<<"')";
        sql=ostr.str();
        bool ret=database.query_sql(sql);

        ostr.str("");
        if(a)
        {
            std::stringstream ss;
            ss<<uid1;
            int useruid;
            ss>>useruid;
            cout<<"UserUid is "<<useruid<<endl;
            iter=userlist.find(useruid);
            strcpy(result1,"friend_request|");
            strcat(result1,uid);
            strcat(result1,"|");
            strcat(result1,request);
        if(iter!=userlist.end())
        {   
            int aa=send(iter->second,&result1,strlen(result1),0);
            cout<<"发送给id="<<iter->second<<" data is :"<<result1<<" return is "<<aa<<" strlen is "<<strlen(result)<<endl;

            strcpy(result,"send_message_succ|发送成功！");
        }
        else//对方离线=database.query_sql(
        {
            cout<<"user is not login!!"<<endl;
            strcpy(result,"add_friend_succ|对方离线！");
        }
            strcpy(result,"add_friend_succ|已发送请求！");
        }
        else
        {
            strcpy(result,"add_friend_error|用户不存在！");
        }
        }
        else
        {
            strcpy(result,"add_friend_error|已存在好友！");
        }
        
    }
    else{
        strcpy(result,"token_error|token错误，请重新登录！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void ServerFunc::delFriend(int clientFd,char message[buffSize])
{
 char result[buffSize]={0};
    char uid[1024]={0},token[1024]={0},uid1[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%s",uid,uid1,token);
    bool a =checktoken(uid,token);
    if(a)
    {
        ostringstream ostr;
        ostr<<"DELETE FROM friend_info WHERE (userid ='"<<uid<<"' AND friend_id = '"<<uid1<<"') OR (userid = '"<<uid1<<"' AND friend_id = '"<<uid<<"')";
        string sql=ostr.str();
        bool a=database.query_sql(sql);
        ostr.str("");
        if(a)
        {
            strcpy(result,"del_friend_succ|删除成功！");
        }
        else
        {
            strcpy(result,"del_friend_error|没有这个好友！");
        }
    }
    else{
        strcpy(result,"update_profile_error|token错误，请重新登录！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void ServerFunc::confirmFriendRequest(int clientFd,char message[buffSize])
{
    char result[buffSize]={0};
    char uid[1024]={0},token[1024]={0},flag[4]={0},uid1[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%s",uid,uid1,flag,token);
    bool a =checktoken(uid,token);
    if(a)
    {  
            ostringstream ostr;
            ostr<<"DELETE FROM friend_apply WHERE object_id = '"<<uid<<"' AND applicant_id = '"<<uid1<<"'" ;
            string sql=ostr.str();
            bool ret=database.query_sql(sql);
            ostr.str("");
            if(ret&&flag[0]=='0')
            {ostr<<"INSERT INTO friend_info (userid,friend_id) VALUES ('"<<uid<<"','"<<uid1<<"')" ;
            sql=ostr.str();
            string nickname=database.query(sql);
            ostr.str("");
            ostr<<"INSERT INTO friend_info (userid,friend_id) VALUES ('"<<uid1<<"','"<<uid<<"')" ;
            sql=ostr.str();
            nickname=database.query(sql);
            ostr.str("");
            ostr<<"INSERT INTO single_chat_info (member_id1,member_id2) VALUES ('"<<uid1<<"','"<<uid<<"')" ;
            sql=ostr.str();
            nickname=database.query(sql);
            ostr.str("");
            strcpy(result,"hand_friend_request_succ|成功同意请求！");
            }
            else if(ret&&flag[0]=='1')
            {
                strcpy(result,"hand_friend_request_succ|成功拒绝请求！");
            }
            else if(ret==false){
                strcpy(result,"hand_friend_request_error|不存在的请求！");
            }
    }
    else{
        strcpy(result,"update_profile_error|token错误，请重新登录！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void ServerFunc::createGroup(int clientFd,char message[buffSize])
{
    char result[buffSize]={0};
    char uid[1024]={0},token[1024]={0},groupName[1024]={0},groupid[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%s",uid,groupName,groupid,token);
    bool a =checktoken(uid,token);
    if(a)
    {
        ostringstream ostr;
        ostr<<"SELECT * FROM group_chat_info WHERE group_chat_info_id = '"<<groupid<<"'";
        string sql=ostr.str();
        ostr.str("");
        string rett=database.query(sql);
        if(rett=="NULL")
        {ostr<<"INSERT INTO group_chat_info (group_chat_info_id,member_id,group_chat_name,group_chat_admin) VALUES ('"<<groupid<<"','"<<uid<<"','"<<groupName<<"',"<<"1)";
        string sql=ostr.str();
        bool ret=database.query_sql(sql);
        ostr.str("");
        if(ret)
        {
            strcpy(result,"create_group_succ|");
            strcat(result,groupid);
            strcat(result,"|群组创建成功");
        }
        else
        {
            strcpy(result,"create_group_error|群组创建失败！");
        }
        }
        else
        {
            strcpy(result,"create_group_error|群号已被使用！");
        }
        
    }
    else{
        strcpy(result,"update_profile_error|token错误，请重新登录！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void ServerFunc::joinGroup(int clientFd,char message[buffSize])
{
    char result[buffSize]={0},result1[buffSize]={0};
    char uid[1024]={0},token[1024]={0},groupId[1024]={0},request[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%s",uid,groupId,request,token);
    bool a =checktoken(uid,token);
    if(a)
    {
        ostringstream ostr;
        ostr<<"INSERT INTO group_chat_management (applicant_id,group_chat_id,group_chat_management_message) VALUES ('"<<uid<<"','"<<groupId<<"','"<<request<<"')";
        string sql=ostr.str();
        bool ret=database.query_sql(sql);
        ostr.str("");
        if(ret)
        {
            ostr<<"SELECT member_id FROM group_chat_info WHERE group_chat_admin = 1 AND group_chat_id = '"<<groupId<<"'";
            sql=ostr.str();
            string adminidstr=database.query(sql);
            int adminid=stoi(adminidstr);
            iter=userlist.find(adminid);
            if(iter!=userlist.end())
            {
                strcpy(result1,"group_request|");
                strcat(result1,uid);
                strcat(result1,"|");
                strcat(result1,request);
                strcpy(result1,"join_group_succ|已发送请求！");
                send(iter->second,&result1,strlen(result1),0);
                cout<<"发送给id="<<iter->second<<" data is :"<<result1<<endl;
            }
            else
            {
                strcpy(result,"join_group_error|群主离线！");
            }
            
            
        }
        else
        {
            strcpy(result,"join_group_error|群组不存在！");
        }
    }
    else{
        strcpy(result,"token_error|token错误，请重新登录！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void ServerFunc::handleGroupRequest(int clientFd,char message[buffSize])
{
    char result[buffSize]={0};
    char uid[1024]={0},token[1024]={0},flag[4]={0},groupid[1024]={0},uid1[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%[^|]|%s",uid1,uid,groupid,flag,token);
    bool a =checktoken(uid1,token);
    if(a)
    {  
            ostringstream ostr;
            ostr<<"DELETE FROM group_chat_management WHERE group_chat_info_id = '"<<groupid<<"' AND applicant_id = '"<<uid<<"'" ;
            string sql=ostr.str();
            bool ret=database.query_sql(sql);
            ostr.str("");
            if(ret&&flag[0]=='0')
            {ostr<<"INSERT INTO group_chat_info (applicant_id,group_chat_info_id) VALUES ('"<<uid<<"','"<<groupid<<"')" ;
            sql=ostr.str();
            string nickname=database.query(sql);
            ostr.str("");
            strcpy(result,"hand_group_request_succ|成功同意请求！");
            }
            else if(ret&&flag[0]=='1')
            {
                strcpy(result,"hand_group_request_succ|成功拒绝请求！");
            }
            else if(ret==false){
                strcpy(result,"hand_group_request_error|不存在的请求！");
            }
    }
    else{
        strcpy(result,"update_profile_error|token错误，请重新登录！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void ServerFunc::getUnreadFriendRequest(int clientFd,char message[buffSize])
{
     char result[buffSize]={0};
    char uid[1024]={0},token[1024]={0};
    sscanf(message,"%[^|]|%s",uid,token);
    bool a =checktoken(uid,token);
    if(a)
    {

        ostringstream ostr;
        ostr<<"SELECT COUNT(applicant_id) FROM friend_apply WHERE object_id = '"<<uid<<"'";
        string sql=ostr.str();
        ostr.str("");
        string countStr=database.query(sql);
        int unreadCount=stoi(countStr);
        string uidd,request;
        strcpy(result,"unread_friend_request|");
        strcat(result,countStr.c_str());
        for(int i=0;i<unreadCount;i++)
        {   strcat(result,"|");
            ostr<<"SELECT applicant_id FROM friend_apply WHERE object_id = '"<<uid<<"'";
            string sql=ostr.str();
            ostr.str("");
            uidd=database.query(sql);ostr<<"SELECT friend_apply_message FROM friend_apply WHERE object_id = '"<<uid<<"'";
            sql=ostr.str();
            ostr.str("");
            request=database.query(sql);
            strcat(result,uidd.c_str());
            strcat(result,"|");
            strcat(result,request.c_str());
        }     
    }
    else
    {
       strcpy(result,"update_profile_error|token错误，请重新登录！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
void ServerFunc::getUnreadGroupRequest(int clientFd,char message[buffSize])
{
        char result[buffSize]={0};
    char uid[1024]={0},token[1024]={0};
    sscanf(message,"%[^|]|%s",uid,token);
    bool a =checktoken(uid,token);
    if(a)
    {
        ostringstream ostr;
        ostr<<"SELECT COUNT(*) FROM group_chat_management,group_chat_info WHERE group_chat_info.group_chat_info_id = group_chat_management.group_chat_id AND member_id = '"<<uid<<"' AND group_chat_admin = 1";
        string sql=ostr.str();
        ostr.str("");
        string countStr=database.query(sql);
        int unreadCount=stoi(countStr);
        string uidd,request;
        strcpy(result,"unread_group_request|");
        strcat(result,countStr.c_str());
        for(int i=0;i<unreadCount;i++)
        {   strcat(result,"|");
            ostr<<"SELECT applicant_id FROM group_chat_management,group_chat_info WHERE group_chat_info.group_chat_info_id = group_chat_management.group_chat_id AND member_id = '"<<uid<<"' AND (group_chat_admin = 1 OR group_chat_manager = 1)";
            sql=ostr.str();
            string applyId=database.query(sql);
            ostr.str("");
            ostr<<"SELECT group_chat_info_id FROM group_chat_management,group_chat_info WHERE group_chat_info.group_chat_info_id = group_chat_management.group_chat_id AND member_id = '"<<uid<<"' AND (group_chat_admin = 1 OR group_chat_manager = 1)";
            sql=ostr.str();
            ostr.str("");
            string groupID=database.query(sql);
            ostr<<"SELECT group_chat_management_message FROM group_chat_management,group_chat_info WHERE group_chat_info.group_chat_info_id = group_chat_management.group_chat_id AND member_id = '"<<uid<<"' AND (group_chat_admin = 1 OR group_chat_manager = 1)";
            string sql=ostr.str();
            ostr.str("");
            request=database.query(sql);
            strcat(result,applyId.c_str());
            strcat(result,"|");
            strcat(result,groupID.c_str());
            strcat(result,"|");
            strcat(result,request.c_str());
        }     
    }
    else
    {
       strcpy(result,"update_profile_error|token错误，请重新登录！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
    }
void ServerFunc::getUnreadMessage(int clientFd,char message[buffSize])
{
        char result[buffSize]={0},result1[buffSize]={0};
    char uid[1024]={0},token[1024]={0};
    sscanf(message,"%[^|]|%s",uid,token);
    bool a =checktoken(uid,token);
    if(a)
    {
        ostringstream ostr;
        ostr<<"SELECT COUNT(poster_id) FROM temperary_message,user  WHERE temperary_message.poster_id = user.userid AND recv_id ='"<<uid<<"' ORDER BY temperary_message.time DESC";
        string sql=ostr.str();
        ostr.str("");
        string countp2p=database.query(sql);
        int unreadCount=stoi(countp2p);
        string uidd,request;

        for(int i=0;i<unreadCount;i++)
        {     
            strcpy(result1,"unread_message|0|");
            
            ostr<<"SELECT user.userid FROM temperary_message,user  WHERE temperary_message.poster_id = user.userid AND recv_id ='"<<uid<<"' ORDER BY temperary_message.time DESC";
            string sql=ostr.str();
            ostr.str("");
            uidd=database.query(sql);
            ostr<<"SELECT content FROM temperary_message,user  WHERE temperary_message.poster_id = user.userid AND recv_id ='"<<uid<<"' ORDER BY temperary_message.time DESC";
            sql=ostr.str();
            ostr.str("");
            request=database.query(sql);
            ostr<<"SELECT time FROM temperary_message,user  WHERE temperary_message.poster_id = user.userid AND recv_id ='"<<uid<<"' ORDER BY temperary_message.time DESC";
            sql=ostr.str();
            ostr.str("");
            string time=database.query(sql);
            strcat(result1,uidd.c_str());
            strcat(result1,"|");
            strcat(result1,time.c_str());
            strcat(result1,"|");
            strcat(result1,request.c_str());
            send(clientFd,&result1,strlen(result1),0);
            cout<<"发送给id="<<clientFd<<" data is :"<<result1<<endl;
        }
        ostr<<"SELECT COUNT(group_chat_info_id) FROM temperary_message ,user, group_chat_info WHERE temperary_message.poster_id = user.userid AND group_chat_info.group_chat_info_id = temperary_message.group_chat_info_id AND recv_id = '"<<uid<<"' ORDER BY temperary_message.time DESC";
        sql=ostr.str();
        ostr.str("");
        countp2p=database.query(sql);
        unreadCount=stoi(countp2p);

        for(int i=0;i<unreadCount;i++)
        {     
            strcpy(result1,"unread_message|1|");
            
            ostr<<"SELECT group_chat_info_id FROM temperary_message ,user, group_chat_info WHERE temperary_message.poster_id = user.userid AND group_chat_info.group_chat_info_id = temperary_message.group_chat_info_id AND recv_id = '"<<uid<<"' ORDER BY temperary_message.time DESC";
            string sql=ostr.str();
            ostr.str("");
            uidd=database.query(sql);
            ostr<<"SELECT content FROM temperary_message ,user, group_chat_info WHERE temperary_message.poster_id = user.userid AND group_chat_info.group_chat_info_id = temperary_message.group_chat_info_id AND recv_id = '"<<uid<<"' ORDER BY temperary_message.time DESC";
            sql=ostr.str();
            ostr.str("");
            request=database.query(sql);
            ostr<<"SELECT time FROM temperary_message ,user, group_chat_info WHERE temperary_message.poster_id = user.userid AND group_chat_info.group_chat_info_id = temperary_message.group_chat_info_id AND recv_id = '"<<uid<<"' ORDER BY temperary_message.time DESC";
            sql=ostr.str();
            ostr.str("");
            string time=database.query(sql);
            strcat(result1,uidd.c_str());
            strcat(result1,"|");
            strcat(result1,time.c_str());
            strcat(result1,"|");
            strcat(result1,request.c_str());
            send(clientFd,&result1,strlen(result1),0);
            cout<<"发送给id="<<clientFd<<" data is :"<<result1<<endl;
        }  

            ostr<<"DELETE FROM temperary_message WHERE recv_id = '"<<uid<<"'";
            sql=ostr.str();
            database.query_sql(sql);    
    }
    else
    {
       strcpy(result,"update_profile_error|token错误，请重新登录！");
           send(clientFd,&result,strlen(result),0);
        cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
    }

}
void ServerFunc::sendFile(int clientFd,char message[buffSize])
{
    char result[buffSize]={0},result1[buffSize];
    char uid[1024]={0},token[1024]={0},uid1[1024]={0},el[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%s",uid,token,uid1,el);
    bool a =checktoken(uid,token);
    if(a)
    {
        stringstream ss;
        ss<<uid1;
        int receuid;
        ss>>receuid;
        iter=userlist.find(receuid);
        if(iter!=userlist.end())
        {
            strcpy(result1,"file|");
            strcat(result1,uid);
            strcat(result1,el);
            send(iter->second,&result1,strlen(result1),0);
            cout<<"发送给id="<<iter->second<<" data is :"<<result1<<endl;
            strcpy(result,"send_file|已发送请求");
        }
        else
        {
            strcpy(result,"send_file|对方离线");
        }
        
    }
    else
    {
       strcpy(result,"update_profile_error|token错误，请重新登录！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
}
#endif