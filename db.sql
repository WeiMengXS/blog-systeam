

create database if not exists blog_systeam;

use blog_systeam;

create table if not exists table_tag(
    id int primary key auto_increment comment "标签名称",
      name varchar(20)

);
create table if not exists table_user(
    id int primary key auto_increment comment "用户名称",
      name varchar(20),
        unique key (name)

);


create table if not exists table_blog(
    id int primary key auto_increment comment '博客id',
      tag_id int comment "所属标",
        user_id int comment '用户id',
          title varchar(256) comment '博客标题',
            content text comment '正文',
              ctime datetime comment '博客最后更新时间',
                foreign key (tag_id) references table_tag(id),
                  foreign key (user_id) references table_user(id)
                    

);
