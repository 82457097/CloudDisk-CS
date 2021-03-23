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
#include"logger.h"

#define BUFFER_SIZE 1024
#define PORT 8888
#define MAX_CONNECT 20

using namespace std;

class Socket {
public:
	Socket();
	
	~Socket() {}

	void SockInitServer();
	
	void SockInitClient();
	
	bool SockBind();
	
	bool SockListen();
	
	int SockConnect();
	
	static int SockConnect(int sockFd, struct sockaddr_in &sockAddr);
	
	static int SockAccpet(int sockFd, struct sockaddr_in &sockAddr);
	
	static int SockRecv(int recvfd, char buf[], int len);
	
	static int SockSend(int sendfd, const char buf[], int len);
	
	bool SockClose();
	
	static bool SockClose(int fd);

	int m_sockfd;
    int recvLen;
	struct sockaddr_in m_sockaddr;
};



