blog_systeam:blog_systeam.cc db.hpp
	g++ -std=c++11 -g $^ -o $@ -L/usr/lib64/mysql -ljsoncpp -lmysqlclient -lpthread

