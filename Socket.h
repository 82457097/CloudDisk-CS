#include<sys/socket.h>
#include<sys/types.h>
#include<cstring>
#include<netinet/in.h>
#include<memory.h> 
#include<cstdlib> 
#include<iostream>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<arpa/inet.h> 
#include"file.h"
#include"logger.h"

#define BUFFER_SIZE 1024
#define PORT 8888
#define MAX_CONNECT 20

using namespace std;

class Socket {
public:
	
	Socket();
	~Socket() {}

	bool SockInitServer();
	bool SockInitClient();
	bool SockBind();
	bool SockListen();
	int SockConnect();
	int SockAccpet();
	int SockRecv(int recvfd, char buf[], int len);
	int SockSend(int sendfd, const char buf[], int len);
	bool SockClose();

	int m_sockfd;
	int recvLen;
	int sendLen;
	struct sockaddr_in m_sockaddr;
};



