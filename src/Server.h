#pragma once
#include "Socket.h"
#include "ConnPool.h"
#include "file.h"
#include "fastDFS.h"
#include "Epoll.h"

class Server {
public:

	Socket socket;
	File file;
	FastDFS fastDFS;

	Server() {}
	~Server() {}

	bool ServerInit();
	bool ServerAccept();
	bool GetFileName();
	bool WriteFile(int recvfd, int fd);
	bool UploadFile();
	bool SaveToMysql(Statement* state, string fileName, string fileId);
	
};
