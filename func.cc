#include "func.h"
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string>
#include<errno.h>
#include<mysql/mysql.h>
using namespace std;
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

   connectMysql("localhost","root","Z001221z","user");
}
bool SqlServer::query_sql(string sql)
{
    if(!mysql_query(connect,sql.data()))
    {
        return true;
    }
    else{
        cout<<"执行sql出错"<<endl;
        mysql_close(connect);
        exit(0);
    }


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
            result_row = mysql_fetch_row(res_ptr);
            if(result_row!=NULL)
            {
              //  cout<<"line58"<<endl;
                temp = result_row[0];
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
    return temp;
    
}