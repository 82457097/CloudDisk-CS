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

	static bool SockInitServer();
	static bool SockInitClient();
	static bool SockBind();
	static bool SockListen();
	static int SockConnect();
	static int SockAccpet();
	static int SockRecv(int recvfd, char buf[], int len);
	static int SockSend(int sendfd, const char buf[], int len);
	bool SockClose();
	static bool SockClose(int fd);

	int m_sockfd;
	int recvLen;
	int sendLen;
	struct sockaddr_in m_sockaddr;
};



