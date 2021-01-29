#pragma once
#include"Socket.h"
#include"uploadfile.h"
#include"ConnPool.h"

class Server {
public:

	Socket socket;
	Upload upload;

	Server() {}
	~Server() {}

	bool ServerInit();
	bool ServerAccept();
	bool GetFileName();
	bool WriteFile(int recvfd, int fd);
	
};
