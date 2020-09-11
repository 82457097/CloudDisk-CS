#include"Socket.h"

class Server {
public:

	Socket socket;
	File file;

	Server() {}
	~Server() {}

	bool ServerInit() {
		socket.SockInitServer();
		socket.SockBind();
		file.fileData = (char*)malloc(BUFFER_SIZE);
		memset(file.fileData, '\0', sizeof(file.fileData));
	
		return true;
	}
	
	bool ServerRecv() {
		unsigned int addrLen = sizeof(struct sockaddr);
		int recvLen = socket.SockRecvFrom(file.filePath, 100, &addrLen);
		cout << "filepath is: " << file.filePath << endl;
		if(recvLen < 0) {
			cout << "recv error." << endl;
			return false;
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
		cout  << "filename: " << file.fileName << endl;
		int fd = open(file.fileName, O_CREAT | O_RDWR, 0664);
		if(fd < 0) {
			cout << "open file failed." << endl;
		} else {
			int times = 1;
			while(recvLen = socket.SockRecvFrom(file.fileData, BUFFER_SIZE, &addrLen)) {
				cout << "times: " << times << endl;
				cout << recvLen << endl;
				++times;
				if(recvLen < 0) {
					cout << "recv2 error." << endl;
					break;
				}
				write(fd, file.fileData, recvLen);
				if(recvLen < BUFFER_SIZE) {
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
	
		return true;
	}
	

};
