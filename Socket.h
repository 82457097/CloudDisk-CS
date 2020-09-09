#include<cstdio>
#include<sys/socket.h>
#include<sys/types.h>
#include<cstring>
#include<netinet/in.h>
#include<errno.h>
#include<memory.h> 
#include<cstdlib> 

#define BUFFER_SIZE = 1024;
#define PORT = 8888;

using namespace std;

class Socket {
public:
	
	Socket();

	~Socket() {}

	bool sockInit();

	bool sockBind();

	bool sockRecvFrom();

	bool sockClose();

	bool sockSendTo();
	
	int m_sockfd;
	int fileTrans;
	char* dataBuf;
	int writeLen;
	struct sockaddr_in server, client;
	char fileName[100];
	char filePath[100];
}







