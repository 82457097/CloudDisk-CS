#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <netinet/in.h>
#include <errno.h>
#include <memory.h> 
#include <cstdlib> 

#define BUFFER_SIZE = 1024;
#define PORT = 8888;

using namespace std;

class Socket {
public:
	

	Socket() {
		//int sockfd;
		if((m_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			cout << "socket build success." << endl;
		} else {
			cout << "socket build failed." << endl;
		}
	}

	bool sockInit() {
		char* dataBuf;
		int fileTrans;

		dataBuf = (char*)malloc(BUFFER_SIZE);
		memset(dataBuf, 0, sizeof(dataBuf));
		memset(&server, 0, sizeof(server));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = htonl(INADDR_ANY);
		server.sin_port = htons(PORT);
	}

	bool sockBind() {
		if(bind(m_sockfd, (struct sockaddr*)&server, sizeof(server)) == -1) {
			cout << "bind error." << endl;
		} else {
			cout << "bind success." << endl;
		}
	}

	bool sockRecvFrom() {
		string fileName;
		string filePath;
		int addrLen = sizeof(struct sockaddr);
		int bufLen = recvfrom(m_sockfd, filePath, 100, 0,(struct sockaddr*)&client, &addrLen);
		cout << "filepath is " << filePath << endl;
		if(filePath < 0) {
			cout << "recv error." << endl;
			return -1;
		} else {
			int i = 0, k = 0;
			for(i = strlen(filePath); i >= 0; --i) {
				if(filePath[i] != '/') {
					++k;
				} else {
					break;
				}
			}
		}
	}

	bool sockClose() {}

	bool sockSendTo() {}


public:
	int m_sockfd;
	struct sockaddr_in server, client;
	
}






