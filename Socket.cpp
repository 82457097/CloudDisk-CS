#include"Socket.h"

Socket::Socket() {
	if((m_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		cout << "socket build success." << endl;
	} else {
		cout << "socket build failed." << endl;
	}
}

bool Socket::sockInit() {
	dataBuf = (char*)malloc(BUFFER_SIZE);
	memset(dataBuf, 0, sizeof(dataBuf));
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);

	return true;
}

bool Socket::sockBind() {
	if(bind(m_sockfd, (struct sockaddr*)&server, sizeof(server)) == -1) {
		cout << "bind error." << endl;
		return false;
	} else {
		cout << "bind success." << endl;
	}

	return true;
}

bool Socket::sockRecvFrom() {
	int addrLen = sizeof(struct sockaddr);
	int bufLen = recvfrom(m_sockfd, filePath, 100, 0,(struct sockaddr*)&client, &addrLen);
	cout << "filepath is: " << filePath << endl;
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
		strcpy(fileName, filePath + (strlen(filePath) - k) + 1);
	}
	cout  << "filename: " << filePath << endl;
	int fd = open(fileName, O_CREAT | O_RDWR, 0664);
	if(fd < 0) {
		cout << "open file failed." << endl;
	} else {
		int times = 1;
		while(fileTrans = recvfrom(m_sockfd, dataBuf, BUFFER_SIZE, 0, 
			(struct sockaddr*)&client, addrLen)) {
			cout << "times: " << times << endl;
			++times;
			if(fileTrans < 0) {
				cout << "recv2 error." << endl;
				break;
			}
			writeLen = write(fd, dataBuf, fileTrans);
			if(fileTrans < BUFFER_SIZE) {
				cout << "write finished." << endl;
				break;
			} else {
				cout << "write success." << endl;
			}
			cout << "continue..." << endl;
			memset(dataBuf, 0, sizeof(dataBuf));
		}
		cout << "recv finished." << endl;
		close(fd);
	}
	sockClose();
	
	return true;

}

bool Socket::sockClose() {
	close(m_sockfd);

	return true;
}

bool Socket::sockSendTo() {

}

