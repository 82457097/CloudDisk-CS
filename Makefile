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
	g++ -std=c++11 -o ./bin/Client ./src/file.h ./src/Client* ./src/Socket* ./src/logger* ./src/fast* \
		-I/usr/include/fastdfs/ -I/usr/include/fastcommon/ \
		-L/usr/lib64 -lmysqlclient -lpthread -lfdfsclient -lfcgi \

Server:
	g++ -std=c++11 -o ./bin/Server ./src/file.h ./src/Server* ./src/Socket* ./src/logger* ./src/fast* ./src/ConnPool* ./src/Epoll.* \
        -lmysqlcppconn  -L/usr/local/lib \
		-I/usr/include/fastdfs/ -I/usr/include/fastcommon/ \
		-L/usr/lib64 -lmysqlclient -lpthread -lfdfsclient -lfcgi \

.PHONY: Clean
Clean:
	rm ./bin/Client ./bin/Server
