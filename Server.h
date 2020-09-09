#pragma once
#include"Socket.h"

class Server {
public:
	Server() {}
	~Server() {}

	bool ServerInit() {
		socket.SockInit();
		socket.SockBind();
	
		return true;
	}
	bool ServerRecv() {
		socket.SockRecvFrom();
	
		return true;
	}
	bool ParseMsg() {
		return true;
	}

	Socket socket;
};
