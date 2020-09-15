#include"Socket.h"

Socket::Socket() {
	if((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cout << "socket build failed." << endl;
	} else {
		cout << "socket build success." << endl;
	}
}

bool Socket::SockInitServer() {
	memset(&m_sockaddr, 0, sizeof(m_sockaddr));
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_sockaddr.sin_port = htons(PORT);
	cout << "init success." << endl;

	return true;
}

bool Socket::SockInitClient() {
	memset(&m_sockaddr, 0, sizeof(m_sockaddr));
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_port = htons(PORT);
	m_sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	return true;
}

bool Socket::SockBind() {
	if(bind(m_sockfd, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr)) == -1) {
		cout << "bind error." << endl;
		return false;
	} else {
		cout << "bind success." << endl;
	}

	return true;
}

bool Socket::SockListen() {
	if(listen(m_sockfd, MAX_CONNECT) == -1) {
		cout << "listen error." << endl;
		return false;
	} else {
		cout << "listen success." << endl;
	}

	return true;
}

int Socket::SockConnect() {
	int sendfd;
	if((sendfd = connect(m_sockfd, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr))) == -1) {
		cout << "connect failed." << endl;
	} else {
		cout << "connect success." << endl;
	}


	return sendfd;
}

int Socket::SockAccpet() {
	int recvfd;
	unsigned int addrLen = sizeof(m_sockaddr);
	if((recvfd = accept(m_sockfd, (struct sockaddr*)&m_sockaddr, &addrLen)) == -1) {
		cout << "accept fialed." << endl;
	} else {
		cout << "accept success." << endl;
	}

	return recvfd;
}

int Socket::SockRecv(int recvfd, char buf[], int len) {
	recvLen = recv(recvfd, buf, len, 0);
	//cout << buf << endl;
	if(recvLen < 0) {
		cout << "recv error." << endl;
		return -1;
	} else {
		cout << "recv success." << endl;
	}
	
	return recvLen;
}

int Socket::SockSend(int sendfd, const char buf[], int len) {
	sendLen = send(sendfd, buf, len, 0);
	//cout << buf << endl;
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



