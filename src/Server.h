#pragma once
#include "ConnPool.h"
#include "file.h"
#include "fastDFS.h"
#include "Epoll.h"

class Server {
public:

	//Socket socket;
	File file;
	FastDFS fastDFS;

	Server() {}
	~Server() {}

	bool ServerInit();
	//bool ServerAccept();
	bool GetFileName();
	//bool WriteFile(Socket socket,int recvfd, int fd);
	bool UploadFile();
	bool SaveToMysql(Statement* state, string fileName, string fileId);
	
};
