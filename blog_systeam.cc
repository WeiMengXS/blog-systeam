#include "db.hpp"
#include "httplib.h"
//int test()
//{
//MYSQL* mysql=blog_systream::Mysqlinit();
//blog_systream::TableUser table_user(mysql);

//Json::Value user;
//user["name"]="陈东瑞";
//table_user.Insert(user);
//user.clear();
//user["name"]="蔡自桂";
/*table_user.Insert(user);
user.clear();
user["name"]="马新航";
table_user.Insert(user);

table_user.GetOne(5,&user);
table_user.Delete(5);
Json::Reader reader;
Json::StyledWriter writer;

std::cout<<writer.write(user)<<std::endl;


blog_systream::MysqlRelease(mysql);

  return 0;
}i
*/
blog_systream::TableUser *table_user;

blog_systream::TableTag  *table_tag;
blog_systream::TableBlog *table_blog;

void InsertUser(const httplib::Request &req,httplib::Response &rsp)
{
  //用户信息在req的body中。
  //1.获取json字符串
  std:: string json_user=req.body;
  Json::Reader reader;
  Json::Value root;
  bool ret=reader.parse(json_user,root);
  if(ret==false)
  {
    printf("InsertUser body info json error");
    rsp.status=400;//请求错误
    return ;
  }

  //2.江字符串转换为VALU格式
  //3。调用接口修改数据库
  ret=table_user->Insert(root);
  if(ret==false){
    printf("insert user value error");
    rsp.status=500;
    return ;
  }
  rsp.status=200;
  //4.填充rsp响应
  return ;
}

void DeleteUser(const httplib::Request &req,httplib::Response &rsp)
{
  int user_id =std::stoi(req.matches[1]);

  bool ret=table_user->Delete(user_id);
  if(ret==false)
  {
    printf("Delete info user seq error");
    rsp.status=500;
    return ;
  }
rsp.status=200;
  return ;
}
void UpdateUser(const httplib::Request &req,httplib::Response &rsp)
{
std::cout<<"更新用户"<<std::endl;
  int user_id =std::stoi(req.matches[1]);
  std::string json_user =req.body;
  Json::Reader reader;
  Json::Value root;
  bool ret =reader.parse(json_user,root);
  rsp.status=200;
  if(ret==false)
  {
    printf("Update js error");
    rsp.status=400;
    return ;
  }
  ret=table_user->Update(user_id,root);
if(ret==false)
{
  printf("server is error update error");
  rsp.status=500;
  return ;
}
rsp.status=200;
  return ;
}
void GetOneUser(const httplib::Request &req,httplib::Response &rsp)
{
  std::cout<<"获取单个用户"<<std::endl;
  Json::Value root;
  int user_id=stoi(req.matches[1]);

  bool ret=table_user->GetOne(user_id,&root);
  
  if(ret==false)
  {
    printf("Get one for server is error");
    rsp.status=500;
    return ;
  }
  Json::FastWriter writer;
  rsp.set_content(writer.write(root),"application/json");
    return ;
}
void GetAllUser(const httplib::Request &req,httplib::Response &rsp)
{
  std::cout<<"获取所有用户"<<std::endl;
  Json::Value root;
  bool ret=table_user->GetAll(&root);
  if(ret==false)
  {
    printf("Get all is error json");
    rsp.status=500;
    return ;
  }
  Json::FastWriter writer;
  std::string body;
  body=writer.write(root);
  rsp.set_content(body,"application/json");
  return ;
}

void InsertTag(const httplib::Request &req,httplib::Response &rsp)
{
  std::cout<<"插入单个标签"<<std::endl;
  std::string json_tag=req.body;
  Json::Value root;
  Json::Reader reader;
  bool ret=reader.parse(json_tag,root);
  if(ret==false)
  {
    printf("Insert tag is error reader");
    rsp.status=400;
    return ;
  }
  ret=table_tag->Insert(root);
  if(ret==false){
  
    printf("Insert tag is db error");
    rsp.status=500;
    return ;
  }
  rsp.status=200;
  return ;
}

void DeleteTag(const httplib::Request &req,httplib::Response &rsp)
{
  std::cout<<"删除单个标签"<<std::endl;
  int tag_id=std::stoi(req.matches[1]);

  bool ret=table_tag->Delete(tag_id);
if(ret==false)
{
  printf("Tag delete is error for db");
  rsp.status=500;
  return ;
}
rsp.status=200;
  return ;
}
void UpdateTag(const httplib::Request &req,httplib::Response &rsp)
{
  std::cout<<"更新标签"<<std::endl;
  int tag_id=std::stoi(req.matches[1]);
  std::string json_tag=req.body;
  Json::Reader reader;
  Json::Value root;
  bool ret=reader.parse(json_tag,root);
  if(ret==false)
  {
    printf("Tag update is error for reader");
    rsp.status=400;
    return ;
  }
ret=table_tag->Update(tag_id,root);
if(ret==false)
{
  printf("Tag update is error for db");
  rsp.status=500;
  return ;

}
rsp.status=200;
  return ;
}
void GetOneTag(const httplib::Request &req,httplib::Response &rsp)
{
  std::cout<<"获取单个标签"<<std::endl;
  Json::Value root;
  int tag_id=stoi(req.matches[1]);

  bool ret=table_tag->GetOne(tag_id,&root);
  
  if(ret==false)
  {
    printf("Get tagone for server is error");
    rsp.status=500;
    return ;

  }
  Json::FastWriter writer;
  rsp.set_content(writer.write(root),"application/json");
    return ;

}
void GetAllTag(const httplib::Request &req,httplib::Response &rsp)
{
  std::cout<<"获取所有标签"<<std::endl;
  Json::Value root;
  bool ret=table_tag->GetAll(&root);
  if(ret==false)
  {
    printf("Get all tag is error json");
    rsp.status=500;
    return ;
  }
  Json::FastWriter writer;
  std::string body;
  body=writer.write(root);
  rsp.set_content(body,"application/json");
  return ;
}
void InsertBlog(const httplib::Request &req,httplib::Response &rsp)
{
  std::cout<<"插入博客"<<std::endl;
  std::string json_str=req.body;
  Json::Value root;
  Json::Reader reader;
  bool ret=reader.parse(json_str,root);
  if(ret==false)
  {
    printf("Insert blog is error reader");
    rsp.status=400;
    return ;
  }
  ret=table_blog->Insert(root);
  if(ret==false){
  
    printf("Insert blog is db error");
    rsp.status=500;
    return ;
  }
  rsp.status=200;
  return ;
}

void DeleteBlog(const httplib::Request &req,httplib::Response &rsp)
{
  std::cout<<"删除博客"<<std::endl;
  int blog_id=std::stoi(req.matches[1]);

  bool ret=table_blog->Delete(blog_id);
if(ret==false)
{
  printf("blog delete is error for db");
  rsp.status=500;
  return ;
}
rsp.status=200;
}
void UpdateBlog(const httplib::Request &req,httplib::Response &rsp)
{
  std::cout<<"更新博客"<<std::endl;
  int blog_id=std::stoi(req.matches[1]);
  std::string json_str=req.body;
  Json::Reader reader;
  Json::Value root;
  bool ret=reader.parse(json_str,root);
  if(ret==false)
  {
    printf("Blog update is error for reader");
    rsp.status=400;
    return ;
  }
ret=table_blog->Updeta(blog_id,root);
if(ret==false)
{
  printf("blog update is error for db");
  rsp.status=500;
  return ;

}
rsp.status=200;
  return ;

}
void GetOneBlog(const httplib::Request &req,httplib::Response &rsp)
{
  std::cout<<"获取单个博客"<<std::endl;
  Json::Value root;
  int blog_id=std::stoi(req.matches[1]);
  bool ret=table_blog->GetOne(blog_id,&root);
  if(ret==false){
printf("GetOneBlog is error 283");
rsp.status=500;
return ;
  }
  Json::FastWriter writer;
  rsp.set_content(writer.write(root),"application");
  rsp.status=200;

  return ;
}
void GetAllBlog(const httplib::Request &req,httplib::Response &rsp)
{
  std::cout<<"获取所有博客"<<std::endl;
  bool ret;
  Json::Value root;
  if(req.has_param("tag_id"))
  {
    int tag_id=std::stoi(req.get_param_value("tag_id"));

    ret=table_blog->GetOne(tag_id,&root);
    if(ret==false)
     {
    printf("blog gettag is error for db");
    rsp.status=500;
    return  ;
      }
  }
else if(req.has_param("user_id")){

    int user_id=std::stoi(req.get_param_value("user_id"));

    ret=table_blog->GetUser(user_id,&root);
      if(ret==false)
      {
       printf("blog getuser is error for db");
        rsp.status=500;
         return  ;
      }
}
  else
  {
    ret = table_blog->GetAll(&root);
    if(ret==false)
    {
      printf("get blog all is error");
      rsp.status=500;
      return ;
    }
      
  }
Json::FastWriter writer;
rsp.set_content(writer.write(root),"application/json");
rsp.status=200;
  return ;
}
#define WWWROT "./www"
int main()
{
MYSQL* mysql=blog_systream::Mysqlinit();
if(mysql==NULL)
{
  return -1;
}
table_blog=new blog_systream::TableBlog(mysql);
table_tag=new blog_systream::TableTag(mysql);
table_user=new blog_systream::TableUser(mysql);


httplib::Server server; 
  server.set_base_dir(WWWROT);

  //路由注册处理函数可以是一个正则表达式。
  //正则表达式在：匹配某种规则，特定格式的字符串
  server.Post(R"(/user)",InsertUser);
  server.Delete(R"(/user/(\d+))",DeleteUser);
  server.Put(R"(/user/(\d+))",UpdateUser);
  server.Get(R"(/user)",GetAllUser);
  server.Get(R"(/user/(\d+))",GetOneUser);

  server.Post(R"(/tag)",InsertTag);
  server.Delete(R"(/tag/(\d+))",DeleteTag);
  server.Put(R"(/tag/(\d+))",UpdateTag);
  server.Get(R"(/tag)",GetAllTag);
  server.Get(R"(/tag/(\d+))",GetOneTag);

  server.Post(R"(/blog)",InsertBlog);
  server.Delete(R"(/blog/(\d+))",DeleteBlog);
  server.Put(R"(/blog/(\d+))",UpdateBlog);
  server.Get(R"(/blog)",GetAllBlog);
  server.Get(R"(/blog/(\d+))",GetOneBlog);
  server.listen("0.0.0.0",9000);
  blog_systream::MysqlRelease(mysql);
  return 0;
}
