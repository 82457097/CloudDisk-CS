#include"Socket.h"

Socket::Socket() {
	if((m_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		cout << "socket build failed." << endl;
	} else {
		cout << "socket build success." << endl;
	}
}

bool Socket::SockInit() {
	file.fileData = (char*)malloc(BUFFER_SIZE);
	memset(file.fileData, 0, sizeof(file.fileData));
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);

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

bool Socket::SockRecvFrom() {
	unsigned int addrLen = sizeof(struct sockaddr);
	int bufLen = recvfrom(m_sockfd, file.filePath, 100, 0,(struct sockaddr*)&client, &addrLen);
	cout << "filepath is: " << file.filePath << endl;
	if(file.filePath < 0) {
		cout << "recv error." << endl;
		return -1;
	} else {
		int i = 0, k = 0;
		for(i = strlen(file.filePath); i >= 0; --i) {
			if(file.filePath[i] != '/') {
				++k;
			} else {
				break;
			}
		}
		strcpy(file.fileName, file.filePath + (strlen(file.filePath) - k) + 1);
	}
	cout  << "filename: " << file.filePath << endl;
	int fd = open(file.fileName, O_CREAT | O_RDWR, 0664);
	if(fd < 0) {
		cout << "open file failed." << endl;
	} else {
		int times = 1;
		while(file.fileTrans = recvfrom(m_sockfd, file.fileData, BUFFER_SIZE, 0, 
			(struct sockaddr*)&client, &addrLen)) {
			cout << "times: " << times << endl;
			++times;
			if(file.fileTrans < 0) {
				cout << "recv2 error." << endl;
				break;
			}
			file.writeLen = write(fd, file.fileData, file.fileTrans);
			if(file.fileTrans < BUFFER_SIZE) {
				cout << "write finished." << endl;
				break;
			} else {
				cout << "write success." << endl;
			}
			cout << "continue..." << endl;
			memset(file.fileData, 0, sizeof(file.fileData));
		}
		cout << "recv finished." << endl;
		close(fd);
	}
	SockClose();
	
	return true;

}

bool Socket::SockClose() {
	close(m_sockfd);

	return true;
}

bool Socket::SockSendTo() {

}

