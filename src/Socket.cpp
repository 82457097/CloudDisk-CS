#include"Socket.h"

Socket::Socket() {

}

void Socket::SockInitServer() {
	if((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		LOG("socket build failed.");
	} else {
		LOG("socket build success.");
	}

	memset(&m_sockaddr, 0, sizeof(m_sockaddr));
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_port = htons(PORT);
	m_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
}

void Socket::SockInitClient() {
	if((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		LOG("socket build failed.");
	} else {
		LOG("socket build success.");
	}

	memset(&m_sockaddr, 0, sizeof(m_sockaddr));
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_port = htons(PORT);
	m_sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
}

bool Socket::SockBind() {
	if(bind(m_sockfd, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr)) == -1) {
		LOG("bind error.");
		return false;
	} else {
		LOG("bind success.");
	}

	return true;
}

bool Socket::SockListen() {
	if(listen(m_sockfd, MAX_CONNECT) == -1) {
		LOG("listen error.");
		return false;
	} else {
		LOG("listen success.");
	}

	return true;
}

int Socket::SockConnect() {
	int sendfd;
	if((sendfd = connect(m_sockfd, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr))) == -1) {
		LOG("connect failed.");
	} else {
		LOG("connect success.");
	}


	return sendfd;
}

int Socket::SockConnect(int sockFd, struct sockaddr_in &sockAddr) {
	int sendfd;
	if((sendfd = connect(sockFd, (struct sockaddr*)&sockAddr, sizeof(sockAddr))) == -1) {
		LOG("connect failed.");
	} else {
		LOG("connect success.");
	}


	return sendfd;
}

int Socket::SockAccpet(int sockFd, struct sockaddr_in &sockAddr) {
	int recvfd;
	unsigned int addrLen = sizeof(sockAddr);
	if((recvfd = accept(sockFd, (struct sockaddr*)&sockAddr, &addrLen)) == -1) {
		LOG("accept fialed.");
	} else {
		LOG("accept success.");
	}

	return recvfd;
}

int Socket::SockRecv(int recvfd, char buf[], int len) {
	int recvLen = recv(recvfd, buf, len, 0);
	if(recvLen < 0) {
		LOG("recv error.");
		return -1;
	} else {
		LOG("recv success.");
	}
	
	return recvLen;
}

int Socket::SockSend(int sendfd, const char buf[], int len) {
	int sendLen = send(sendfd, buf, len, 0);
	if(sendLen < 0) {
		LOG("send failed.");
		return -1;
	} else {
		LOG("send success.");
	}

	return sendLen;
}

bool Socket::SockClose(int fd) {
	close(fd);
	LOG("close socket: %d success.", fd);

	return true;
}


bool Socket::SockClose() {
	close(m_sockfd);
	LOG("close success.");
	
	return true;
}



