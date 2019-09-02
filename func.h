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
            int i;
            string str11="login_succ|"+uid+"|"+token;
            for( i=0;i<str11.length();i++)
                result[i] = str11[i];
                result[i] = '\0';string ret=database.query(sql);
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
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result<<endl;
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
        strcpy(result1_char,result1.c_str());
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
    }
    else 
    {
        strcpy(result1_char,"get_user_friend|您无权进行此操作！");
    }
    send(clientFd,&result,strlen(result),0);
    cout<<"发送给id="<<clientFd<<" data is :"<<result1_char<<endl;
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
        else//对方离线=database.query_sql(
        {
            cout<<"user is not login!!"<<endl;
        }
    }
    else
    {
        strcpy(result1,"update_profile_error|token错误，请重新登录！");
    }
    send(clientFd,&result1,strlen(result),0);
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
    char uid[1024]={0},token[1024]={0};
    sscanf(message,"%[^|]|%s",uid,token);
    bool a =checktoken(uid,token);
    if(a)
    {ostringstream ostr;
    ostr<<"SELECT nickname FROM user WHERE userid = '"<<uid<<"'";
    string sql=ostr.str();
    string nickname=database.query(sql);
    ostr.str("");
    ostr<<"SELECT head_portrait_id FROM user WHERE userid = '"<<uid<<"'";
    sql=ostr.str();
    string hid=database.query(sql);
    ostr.str("");
    ostr<<"SELECT signature FROM user WHERE userid = '"<<uid<<"'";
    sql=ostr.str();
    string sig=database.query(sql);
    ostr.str("");
    strcpy(result,"userinfo|");
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
        ostr<<"SELECT COUNT(group_chat_info_id=database.query_sql( FROM group_chat_info WHERE member_id = '"<<uid<<"'";
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
        for (int i=0;i<gCount;i++)
        {
            strcat(result,"|");
            string groupid=database.query(sql,i);
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
        strcpy(result,"groupinfo|");
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
    char uid[1024]={0},token[1024]={0},uid1[1024]={0},request[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%s",uid,uid1,request,token);
    bool a =checktoken(uid,token);
    if(a)
    {
        ostringstream ostr;
        ostr<<"INSERT INTO friend_apply (object_id,applicant_id,friend_apply_message) VALUES('"<<uid<<"','"<<uid1<<"','"<<request<<"')";
        string sql=ostr.str();
        bool ret=database.query_sql(sql);
        ostr.str("");
        if(a)
        {
            strcpy(result,"add_friend_succ|已发送请求！");
        }
        else
        {
            strcpy(result,"add_friend_error|用户不存在！");
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
    char uid[1024]={0},token[1024]={0},groupName[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%s",uid,groupName,token);
    bool a =checktoken(uid,token);
    if(a)
    {
        ostringstream ostr;
        ostr<<"INSERT INTO group_chat_info (member_id,group_chat_name,group_chat_admin,group_chat_manager) VALUES ('"<<uid<<"','"<<groupName<<"','"<<"1,1)";
        string sql=ostr.str();
        bool ret=database.query_sql(sql);
        ostr.str("");
        if(ret)
        {
            ostr<<"SELECT group_chat_info_id FROM group_chat_info WHERE group_chat_name = '"<<groupName<<"'";
            sql=ostr.str();
            string groupId=database.query(sql);
            strcpy(result,"create_group_succ|");
            strcat(result,groupId.c_str());
            strcat(result,"|");
        }
        else
        {
            strcpy(result,"create_group_error|groupid|群组创建失败！");
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
    char result[buffSize]={0};
    char uid[1024]={0},token[1024]={0},groupId[1024]={0},request[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%s",uid,groupId,request,token);
    bool a =checktoken(uid,token);
    if(a)
    {
        ostringstream ostr;
        ostr<<"INSERT INTO group_chat_management (applicant_id,group_chat_info_id) VALUES ('"<<uid<<"','"<<groupId<<"','"<<request<<"')";
        string sql=ostr.str();
        bool ret=database.query_sql(sql);
        ostr.str("");
        if(a)
        {
            strcpy(result,"join_group_succ|已发送请求！");
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
    char uid[1024]={0},token[1024]={0},flag[4]={0},groupid[1024]={0};
    sscanf(message,"%[^|]|%[^|]|%[^|]|%s",uid,groupid,flag,token);
    bool a =checktoken(uid,token);
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
#endif