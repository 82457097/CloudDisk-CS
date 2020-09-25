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
	LOG("filepath is: %s", file.filePath);
	if(recvfd < 0) {
		LOG("recv error.");
		return false;
	} else {
		GetFileName();
	}
	LOG("filename: %s", file.fileName);
	
	int fd = open(file.fileName, O_CREAT | O_RDWR, 0664);
	if(fd < 0) {
		LOG("open file failed.");
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
		//cout << "times: " << times << endl;
		//cout << socket.recvLen << endl;
		++times;
		if(socket.recvLen < 0) {
			LOG("recv2 error.");
			break;
		}
		write(fd, file.fileData, socket.recvLen);
		if(socket.recvLen < BUFFER_SIZE) {
			LOG("write finished.");
			break;
		} else {
			LOG("write success.");
		}
		//cout << "continue..." << endl;
		memset(file.fileData, 0, sizeof(file.fileData));
	}
	LOG("recv finished.");
	close(fd);

	return true;
}

int main() {
	Server server;
	if (!server.upload.mysql.MysqlInit()) {
		LOG("Init failed!");
		return -1;
	}
	
	if(server.ServerInit()) {
		LOG("server init success.");
	} else {
		LOG("server init failed.");
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
