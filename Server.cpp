#include"Server.h"

bool Server::ServerInit() {
	socket.SockInitServer();
	socket.SockBind();
	socket.SockListen();
	file.fileData = (char*)malloc(BUFFER_SIZE);
	memset(file.fileData, '\0', sizeof(file.fileData));

	return true;
}
	
bool Server::ServerAccept() {
	int recvfd = socket.SockAccpet();
	cout << socket.SockRecv(recvfd, file.filePath, 100) << endl;
	cout << "filepath is: " << file.filePath << endl;
	if(recvfd < 0) {
		cout << "recv error." << endl;
		return false;
	} else {
		GetFileName();
	}
	cout  << "filename: " << file.fileName << endl;
	
	int fd = open(file.fileName, O_CREAT | O_RDWR, 0664);
	if(fd < 0) {
		cout << "open file failed." << endl;
		return false;
	} else {
		WriteFile(recvfd, fd);
	}

	return true;
}

bool Server::GetFileName() {
	int i = 0, k = 0;
	for(i = strlen(file.filePath); i >= 0; --i) {
		if(file.filePath[i] != '/') {
			++k;
		} else {
			break;
		}
	}
	strcpy(file.fileName, file.filePath + (strlen(file.filePath) - k) + 1);

	return true;
}

bool Server::WriteFile(int recvfd, int fd) {
	int times = 1;
	while(socket.recvLen = socket.SockRecv(recvfd, file.fileData, BUFFER_SIZE)) {
		cout << "times: " << times << endl;
		cout << socket.recvLen << endl;
		++times;
		if(socket.recvLen < 0) {
			cout << "recv2 error." << endl;
			break;
		}
		write(fd, file.fileData, socket.recvLen);
		if(socket.recvLen < BUFFER_SIZE) {
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

	return true;
}

int main() {
	Server server;
	if (!server.upload.mysql.MysqlInit()) {
		cout << "Init failed!" << endl;
		return -1;
	}
	
	if(server.ServerInit()) {
		cout << "server init success." << endl;
	} else {
		cout << "server init failed." << endl;
		return -1;
	}

	while(true) {
		if(server.ServerAccept()) {
			server.upload.UploadFile(server.file.fileName);
			server.upload.SaveToMysql();
			continue;
		} else {
			break;
		}
	}

	server.socket.SockClose();

	return 0;
}
