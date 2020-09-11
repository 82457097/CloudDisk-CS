#include"Socket.h"

Socket::Socket() {
	if((m_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		cout << "socket build failed." << endl;
	} else {
		cout << "socket build success." << endl;
	}
}

bool Socket::SockInit() {
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);
	cout << "init success." << endl;

	return true;
}

bool Socket::SockBind() {
	if(bind(m_sockfd, (struct sockaddr*)&server, sizeof(server)) == -1) {
		cout << "bind error." << endl;
		return false;
	} else {
		cout << "bind success." << endl;
	}

	return true;
}

int Socket::SockRecvFrom(char buf[], int len, unsigned int* addrLen) {
	int recvLen = recvfrom(m_sockfd, buf, len, 0, (struct sockaddr*)&client, addrLen);
	if(recvLen < 0) {
		cout << "recv error." << endl;
		return -1;
	} else {
		cout << "recv success." << endl;
	}
	
	return recvLen;
}

int Socket::SockSendTo(const char buf[], int len, int addrLen) {
	int sendLen = sendto(m_sockfd, buf, len, 0, (struct sockaddr *)&server, addrLen);
	if(sendLen < 0) {
		cout << "send failed." << endl;
		return -1;
	} else {
		cout << "send success." << endl;
	}

	return sendLen;
}


bool Socket::SockClose() {
	close(m_sockfd);
	cout << "close success." << endl;
	
	return true;
}



