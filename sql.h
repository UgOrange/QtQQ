#ifndef __SQL_H__
#define __SQL_H__
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include<errno.h>
#include <string>
#include<mysql/mysql.h>
using namespace std;
class SqlServer
{
private:
    /* data */
    MYSQL *connect; //数据库链接指针
    MYSQL_RES *res_ptr;//指向查询结果的指针
    MYSQL_FIELD *field;//字段结构指针
    MYSQL_ROW result_row;//按行返回查询信息
public:
    SqlServer();
    void connectMysql(char *ip,char * user,char* password,char* database);   //连接数据库
    string query(string sql);   
    string query(string sql,int i);   //执行sql语句接口
    string queryFriend(string sql);
    bool query_sql(string sql);
};
SqlServer::SqlServer()
{
    //初始化连接句柄
    connect = mysql_init(NULL);
    if(connect == NULL)
    {
        //返回null初始化失败
        printf("mysql_init failed!\n");
        exit(0);
    }
    cout<<"connect to database\n";
   connectMysql("localhost","root","p/sxggtql","QQ_Database");
}
bool SqlServer::query_sql(string sql)
{
    cout<<sql<<endl;
    if(!mysql_query(connect,sql.data()))
    {
        return true;
    }
    else{
        cout<<"执行sql出错"<<endl;
    }
    return false;

}
string SqlServer::queryFriend(string sql)
{
     string temp = "error";
     string result = "#";
   // cout<<"line44 sql.data "<<sql.data()<<endl;;
    //传入sql语句，将查询结果以char×返回
    
    if(!mysql_query(connect,sql.data()))
    {
        //把查询结果给res_ptr
        res_ptr = mysql_store_result(connect);
        //cout<<"line49"<<endl;
        //如果结果不为空,则输出
        if(res_ptr!=NULL)
        {
            //cout<<"line53"<<endl;
            //cout<<"chax xun hao"<<endl;
            //cout<<"查询结果"<<endl;
            while(result_row=mysql_fetch_row(res_ptr))
            {
                for(int t=0;t<mysql_num_fields(res_ptr);t++)
                {
                    printf("%s\n",result_row[t]);//row就是个数组
                    result.insert(result.size(),result_row[t]);
                    result.insert(result.size()," ");
                }
                result.insert(result.size(),"#");
               // cout<<"line66 result is: "<<result<<endl;
            }
            mysql_free_result(res_ptr);
            
        }
        else{
            //查询结果空
            return temp; 
        }

    }   
    else{
        perror("my_query");
        mysql_close(connect);
        exit(0);
    }
    return result;
    
}
string SqlServer::query(string sql)
{
    string temp = "error";
   cout<<"line44 sql.data "<<sql.data()<<endl<<connect<<endl;;
    //传入sql语句，将查询结果以char×返回
    //if(!mysql_query(connect,sql.data()))
    //{
        //把查询结果给res_ptr
        mysql_query(connect,sql.data());
        res_ptr = mysql_store_result(connect);
        //cout<<"line49"<<endl;
        //如果结果不为空,则输出
        if(res_ptr!=NULL)
        {
            //cout<<"line53"<<endl;
            //cout<<"chax xun hao"<<endl;
            while(result_row = mysql_fetch_row(res_ptr))
            {if(result_row!=NULL)
            {
              //  cout<<"line58"<<endl;
                temp = result_row[0];
            }}
            mysql_free_result(res_ptr);
            
        }
        else{
            //查询结果空
            return temp; 
        }

    //}   
    //else{
        //perror("my_query");
        //mysql_close(connect);
        //exit(0);
    //}
    return temp;
    
}
string SqlServer::query(string sql,int i)
{
    string temp = "error";
   cout<<"line44 sql.data "<<sql.data()<<endl<<connect<<endl;;
    //传入sql语句，将查询结果以char×返回
    //if(!mysql_query(connect,sql.data()))
    //{
        //把查询结果给res_ptr
        mysql_query(connect,sql.data());
        res_ptr = mysql_store_result(connect);
        //cout<<"line49"<<endl;
        //如果结果不为空,则输出
        if(res_ptr!=NULL)
        {
            //cout<<"line53"<<endl;
            //cout<<"chax xun hao"<<endl;
            result_row = mysql_fetch_row(res_ptr);
            if(result_row!=NULL)
            {
                temp = result_row[i];
            }
            mysql_free_result(res_ptr);
            
        }
        else{
            //查询结果空
            return temp; 
        }

    //}   
    //else{
        //perror("my_query");
        //mysql_close(connect);
        //exit(0);
    //}
    return temp;
    
}
void SqlServer::connectMysql(char *ip,char * user,char* password,char* database)
{
    //连接数据库
    connect =mysql_real_connect(connect,ip,user,password,database,0,NULL,0);
    if (connect) {
    printf("Connection success!\n");
  } else {
    printf("Connection failed!\n");
  }
  if(mysql_query(connect,"set names utf8"));
    {   perror("my_query");
    }//防止乱码。设置和数据库的编码一致就不会乱码
}
#endif