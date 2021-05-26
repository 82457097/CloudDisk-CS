All: Client Server
#	g++ -std=c++11 -o Client file.h Client* Socket* logger* fast* uploadfile* MySql* \
#		-I/usr/include/fastdfs/ -I/usr/include/fastcommon/ \
#		-L/usr/lib64 -lmysqlclient -lpthread -lfdfsclient -lfcgi \
#		-D_SVID_SOURCE
#	g++ -std=c++11 -o Server file.h Server* Socket* logger* fast* uploadfile* MySql* \
#		-I/usr/include/fastdfs/ -I/usr/include/fastcommon/ \
#		-L/usr/lib64 -lmysqlclient -lpthread -lfdfsclient -lfcgi \
#		-D_SVID_SOURCE

Client:
	g++ -std=c++11 -o ./bin/Client ./src/file.h ./src/Client* ./src/Socket* ./src/logger* \
			-L/usr/lib64 -lmysqlclient -lpthread

Server:
	g++ -std=c++11 -o ./bin/Server ./src/file.h ./src/Server* ./src/Socket* ./src/logger* ./src/fastDFS* ./src/MysqlConnPool.cpp ./src/MysqlConnPool.h ./src/ProcessPool.h \
        	-lmysqlcppconn  -L/usr/local/lib \
			-I/usr/include/fastdfs/ -I/usr/include/fastcommon/ \
			-L/usr/lib64 -lmysqlclient -lpthread -lfdfsclient 

.PHONY: Clean
Clean:
	rm ./bin/Client ./bin/Server
