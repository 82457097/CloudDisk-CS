#include<cstdio>
#include<sys/socket.h>
#include<sys/types.h>
#include<cstring>
#include<netinet/in.h>
#include<errno.h>
#include<memory.h> 
#include<cstdlib> 
#include<iostream>
#include<fcntl.h>
#include<sys/stat.h>
#include <unistd.h>
#include"file.h"

#define BUFFER_SIZE 1024
#define PORT 8888

using namespace std;

class Socket {
public:
	
	Socket();

	~Socket() {}

	bool SockInit();
	bool SockBind();
	bool SockRecvFrom();
	bool SockClose();
	bool SockSendTo();

	File file;
	int m_sockfd;
	struct sockaddr_in server, client;
};







