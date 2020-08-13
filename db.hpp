#include<iostream>
#include<cstdlib>
#include<string>
#include<cstdio>
#include<jsoncpp/json/json.h>
#include<mysql/mysql.h>
#include<mutex>
#define MYSQL_HOST "127.0.0.1"
#define MYSQL_DB   "blog_systeam"
#define MYSQL_USER "root"
#define MYSQL_PASS "111111"
namespace blog_systream{
  static std::mutex _mutex;
  static MYSQL *Mysqlinit(){
   MYSQL *mysql=NULL;//1初始化mysql句柄
   mysql=mysql_init(NULL);
   if(mysql==NULL){
      printf("mysql init error");
      return NULL;
   }
   if(mysql_real_connect(mysql,MYSQL_HOST,MYSQL_USER,MYSQL_PASS,MYSQL_DB,0,NULL,0)==NULL)
   {//2.连接数据库
     printf("connect error :%s\n",mysql_error(mysql));
     mysql_close(mysql);
     return NULL;
   }
  //3.设置字符编码集
  
   if(mysql_set_character_set(mysql,"utf8")!=0)
   {

    printf("set error :%s\n",mysql_error(mysql));
   mysql_close(mysql);
    return NULL;
   }
   return mysql;
    
  }
  static void MysqlRelease(MYSQL *mysql){
    if(mysql)
    {
      mysql_close(mysql);
      
    }
  }
  static bool Mysqlquery(MYSQL *mysql,const std::string& sql)
  {
    int ret=mysql_query(mysql,sql.c_str());
    if(ret!=0)
    {
  printf("query error :%s is %s\n",sql.c_str(),mysql_error(mysql));
  return false;
    }
    return true;
  }


  class TableTag{
    public:
      TableTag(MYSQL *mysql):_mysql(mysql) {

      }

      bool Insert(const Json::Value &tag){
#define INSERT_TAG "insert table_tag values(null,'%s');"
        char tmp[4096]={0};
        sprintf(tmp,INSERT_TAG,tag["name"].asCString());
        bool ret=Mysqlquery(_mysql,tmp);
        if(ret==false)
          return false;
        return true;
      }


      bool Delete(int tag_id){
#define DROP_TAG "delete from table_tag where id=%d;"
      char tmp[4096]={0};
      sprintf(tmp,DROP_TAG,tag_id);
      bool ret=Mysqlquery(_mysql,tmp);
      if(ret==false)
        return false;
      return true;
      
      }


      bool Update (int tag_id ,const Json::Value &tag){
        
#define UPDATE_TAG  "update table_tag set name='%s' where id=%d;"
      //bin  sprintf(tmp,UPDATE_TAG,tag["name"].asCString(),tag_id);

        char tmp[4096]={0};
      sprintf(tmp,UPDATE_TAG,tag["name"].asCString(),tag_id);
        bool ret=Mysqlquery(_mysql,tmp);
        if(ret==false)
          return false;
        return true;
      }
  
      bool GetAll( Json::Value* tags)
      {
  
#define GETALL_TAG "select * from table_tag;"
        std::lock_guard<std::mutex> lck(_mutex);
  
        bool ret=Mysqlquery(_mysql,GETALL_TAG);
        if(ret==false){
          printf("get all 初始化错误");
          return false;}
        MYSQL_RES * res=mysql_store_result(_mysql);
        int num_row=mysql_num_rows(res);
        if(num_row<0)
        {
          printf("row <=0");
          mysql_free_result(res);
          return false;
        }

      for(int i=0;i<num_row;i++)
      {
        MYSQL_ROW row=mysql_fetch_row(res);
        Json::Value tag;
        tag["id"]=std::stoi(row[0]);
        tag["name"]=row[1];
        tags->append(tag);
      }
          mysql_free_result(res);
return true;
      }

      bool GetOne(int tag_id, Json::Value *tag)
      {
#define GET_ONE "select name from table_tag where id=%d"
        std::lock_guard<std::mutex> lck(_mutex);
        char tmp[1024]={0};
        sprintf(tmp,GET_ONE,tag_id);
        bool ret=Mysqlquery(_mysql,tmp);
        if(ret==false )  return false;
    MYSQL_RES *res=mysql_store_result(_mysql);
    int num_row=mysql_num_rows(res);
    if(num_row<0)
    {
      printf("one error\n");
      mysql_free_result(res);
      return false;
    }
    
    for(int i= 0;i<num_row;i++)
            {
                MYSQL_ROW row=mysql_fetch_row(res);
              
               (*tag)["id"]=tag_id;
                (*tag)["name"]=tag[0];
              
            }
            mysql_free_result(res);
            return true;
      }
    private:
      MYSQL *_mysql;
  };


  class TableUser{
    public:

      TableUser(MYSQL *mysql):_mysql(mysql){ }
     
      bool Insert(const Json::Value &user){
#define INSERT_USER "insert table_user values(null,'%s');"
        char tmp[4096]={0};
        sprintf(tmp,INSERT_USER,user["name"].asCString());
        bool ret= Mysqlquery(_mysql,tmp);
        if(ret==false)
          return false;
        return true;
      }


      bool Delete(int user_id){
#define DROP_USER "delete from table_user where id=%d;"
      char tmp[4096]={0};
      sprintf(tmp,DROP_USER,user_id);
      bool ret=Mysqlquery(_mysql,tmp);
      if(ret==false)
        return false;
      return true;
      
      }


      bool Update (int user_id ,const Json::Value &user){
        
#define UPDATE_USER  "update table_user set name='%s' where id=%d;"
      //bin  sprintf(tmp,UPDATE_USER,user["name"].asCString(),user_id);

        char tmp[4096]={0};
      sprintf(tmp,UPDATE_USER,user["name"].asCString(),user_id);
        bool ret=Mysqlquery(_mysql,tmp);
        if(ret==false)
          return false;
        return true;
      }
  
      bool GetAll( Json::Value* users)
      {
  
#define GETALL_USER "select * from table_user;"
    
        std::lock_guard<std::mutex> lck(_mutex);
        bool ret=Mysqlquery(_mysql,GETALL_USER);
        if(ret==false)
          return false;
        MYSQL_RES * res=mysql_store_result(_mysql);
        int num_row=mysql_num_rows(res);
        if(num_row<=0)
        {
          printf("row <=0");
          mysql_free_result(res);
          return false;
        }

      for(int i=0;i<num_row;i++)
      {
        MYSQL_ROW row=mysql_fetch_row(res);
        Json::Value user;
        user["id"]=std::stoi(row[0]);
        user["name"]=row[1];
        users->append(user);
      }
mysql_free_result(res);
return true;
   }
      bool GetOne(int user_id, Json::Value *user)
      {
#define USER_ONE "select name from table_user where id=%d;"
        std::lock_guard<std::mutex> lck(_mutex);
        char tmp[1024]={0};
        sprintf(tmp,USER_ONE,user_id);
        bool ret=Mysqlquery(_mysql,tmp);
        if(ret==false )  return false;
    MYSQL_RES *res=mysql_store_result(_mysql);
    int num_row=mysql_num_rows(res);
    if(num_row!=1)
    {
      printf("one error");
      mysql_free_result(res);
      return false;
    }
            for(int i= 0;i<num_row;i++)
            {
                MYSQL_ROW row=mysql_fetch_row(res);
              
               (*user)["id"]=user_id;
                (*user)["name"]=row[0];
              
            }
            mysql_free_result(res);
            return true;
      }
      
    private:
      MYSQL *_mysql;
  };
  class TableBlog{
    public:
      TableBlog(MYSQL* mysql):_mysql(mysql){ }
      bool Insert(const Json::Value &blog)
        //id tag_id  user id  title  content ctime
      {
#define INSERT_BLOG "insert table_blog Values(null,%d,%d,'%s','%s',now());"
        char tmp[4096]={0};
sprintf(tmp,INSERT_BLOG,blog["tag_id"].asInt(),blog["user_id"].asInt(),blog["title"].asCString(), blog["content"].asCString());
        bool ret=Mysqlquery(_mysql,tmp);
        if(ret==false)
        {
          return false;
        
        }
        return true;
      }
      bool Delete(int blog_id)
      {

#define DELETE_BLOG "delete from table_blog where id=%d;"
        char tmp[4096]={0};
        sprintf(tmp,DELETE_BLOG,blog_id);
        bool ret=Mysqlquery(_mysql,tmp);
        if(ret==false)
        {
          return false;
        }
        return true;
      }


      bool Updeta(int blog_id,const Json::Value &blog){
#define UPDATE_BLOG_BLOG "update table_blog set tag_id=%d,title='%s',content='%s' where id=%d;"
        char tmp[4096]={0};
        sprintf(tmp,UPDATE_BLOG_BLOG,blog["tag_id"].asInt(),blog["title"].asCString(), blog["content"].asCString(),blog_id);
        bool ret=Mysqlquery(_mysql,tmp);
        if(ret==false)
        {
  printf("Update is error");
          return false;
        }
        return true;
      }

      bool GetAll( Json::Value* blogs)
      {
  
        std::lock_guard<std::mutex> lck(_mutex);
#define GETALL_BLOG "select * from table_blog;"
    
        bool ret=Mysqlquery(_mysql,GETALL_BLOG);
        if(ret==false)
          return false;
        MYSQL_RES * res=mysql_store_result(_mysql);
        int num_row=mysql_num_rows(res);
        if(num_row<=0)
        {
          printf("row <=0");
          mysql_free_result(res);
          return false;
        }

      for(int i=0;i<num_row;i++)
      {
        MYSQL_ROW row=mysql_fetch_row(res);
        Json::Value blog;
        blog["id"]=std::stoi(row[0]);
        blog["tag_id"]=std::stoi(row[1]);
        blog["user_id"]=std::stoi(row[2]);
        blog["title"]=row[3];
        blog["content"]=row[4];
        blog["ctime"]=row[5];
        blogs->append(blog);
      }

mysql_free_result(res);
return true;
      }
     bool GetOne(int blog_id ,Json::Value* blogs)
     {
#define GETONE_ID "select * from table_blog where id=%d;"
    
        std::lock_guard<std::mutex> lck(_mutex);
    char tmp[4096]={0};
    sprintf(tmp,GETONE_ID,blog_id); 
        bool ret=Mysqlquery(_mysql,tmp);
        if(ret==false)
          return false;
        MYSQL_RES * res=mysql_store_result(_mysql);
        int num_row=mysql_num_rows(res);
        if(num_row<=0)
        {
          printf("row <=0");
          mysql_free_result(res);
          return false;
        }

      for(int i=0;i<num_row;i++)
      {
        MYSQL_ROW row=mysql_fetch_row(res);
        Json::Value blog;
        blog["id"]=std::stoi(row[0]);
        blog["tag_id"]=row[1];
        blog["user_id"]=row[2];
        blog["title"]=row[3];
        blog["content"]=row[4];
        blog["ctime"]=row[5];
        blogs->append(blog);
      }

mysql_free_result(res);
return true;
     }
      bool GetTAG(int tag_id, Json::Value* blogs)
      {
  
        std::lock_guard<std::mutex> lck(_mutex);
#define GET_TAG_ALL "select * from table_blog where tag_id=%d;"
    char tmp[4096]={0};
    sprintf(tmp,GET_TAG_ALL,tag_id); 
        bool ret=Mysqlquery(_mysql,tmp);
        if(ret==false)
          return false;
        MYSQL_RES * res=mysql_store_result(_mysql);
        int num_row=mysql_num_rows(res);
        if(num_row<=0)
        {
          printf("row <=0");
          mysql_free_result(res);
          return false;
        }

      for(int i=0;i<num_row;i++)
      {
        MYSQL_ROW row=mysql_fetch_row(res);
        Json::Value blog;
        
        blog["id"]=std::stoi(row[0]);
        blog["tag_id"]=row[1];
        blog["user_id"]=row[2];
        blog["title"]=row[3];
        blog["content"]=row[4];
        blog["ctime"]=row[5];
        blogs->append(blog);
      }
mysql_free_result(res);
return true;
      }
      bool GetUser(int user_id,Json::Value *blogs)
      {
#define GET_USER_ALL "select * from table_blog where user_id=%d;"
        std::lock_guard<std::mutex> lck(_mutex);
        char tmp[4096]={0};
        sprintf(tmp,GET_USER_ALL,user_id);
        bool ret=Mysqlquery(_mysql,tmp);
        if(ret==false)
        {
          return false;
        }
        MYSQL_RES* res=mysql_store_result(_mysql);
        int num_row=mysql_num_rows(res);
        if(num_row<=0)
        {
          printf("Get User blog error ");
          mysql_free_result(res);
          return false;
        }
        for(int i=0;i<num_row;i++)
        {
        MYSQL_ROW row=mysql_fetch_row(res);
        Json::Value blog;

        blog["id"]=std::stoi(row[0]);
        blog["tag_id"]=row[1];
        blog["blog_id"]=row[2];
        blog["title"]=row[3];
        blog["content"]=row[4];
        blog["ctime"]=row[5];
        blogs->append(blog);
        }

mysql_free_result(res);
return true;
      }
    private:
      MYSQL* _mysql;
  };


}
