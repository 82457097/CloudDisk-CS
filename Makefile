
All:
	g++ -std=c++11 -o Client file.h Client* Socket* logger* fast* uploadfile* MySql* \
		-I/usr/include/fastdfs/ -I/usr/include/fastcommon/ \
		-L/usr/lib64 -lmysqlclient -lpthread -lfdfsclient -lfcgi \
		-D_SVID_SOURCE
	g++ -std=c++11 -o Server file.h Server* Socket* logger* fast* uploadfile* MySql* \
		-I/usr/include/fastdfs/ -I/usr/include/fastcommon/ \
		-L/usr/lib64 -lmysqlclient -lpthread -lfdfsclient -lfcgi \
		-D_SVID_SOURCE

Client:
	g++ -std=c++11 -o Client file.h Client.h Client.cpp Socket.h Socket.cpp \
		-I/usr/include/fastdfs/ -I/usr/include/fastcommon/ \
		-L/usr/lib64 -lmysqlclient -lpthread -lfdfsclient \
		-D_SVID_SOURCE
Server:
	g++ -std=c++11 -o Server Server.h Server.cpp Socket.h Socket.cpp file.h \
		-I/usr/include/fastdfs/ -I/usr/include/fastcommon/ \
		-L/usr/lib64 -lmysqlclient -lpthread -lfdfsclient \
		-D_SVID_SOURCE

.PHONY: Clean
Clean:
	rm Client Server
