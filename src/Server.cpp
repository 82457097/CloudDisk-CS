#include"Server.h"

bool Server::ServerInit() {
	socket.SockInitServer();
	socket.SockBind();
	socket.SockListen();
	upload.fileData = (char*)malloc(BUFFER_SIZE);
	memset(upload.fileData, '\0', sizeof(upload.fileData));

	return true;
}
	
bool Server::ServerAccept() {
	int recvfd = socket.SockAccpet();
	socket.SockRecv(recvfd, upload.filePath, 100);
	LOG("filepath is: %s", upload.filePath);
	if(recvfd < 0) {
		LOG("recv error.");
		return false;
	} else {
		GetFileName();
	}
	LOG("filename: %s", upload.fileName);
	
	int fd = open(upload.fileName, O_CREAT | O_RDWR, 0664);
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
	for(i = strlen(upload.filePath); i >= 0; --i) {
		if(upload.filePath[i] != '/') {
			++k;
		} else {
			break;
		}
	}
	strcpy(upload.fileName, upload.filePath + (strlen(upload.filePath) - k) + 1);

	return true;
}

bool Server::WriteFile(int recvfd, int fd) {
	int times = 1;
	while(socket.recvLen = socket.SockRecv(recvfd, upload.fileData, BUFFER_SIZE)) {
		//cout << "times: " << times << endl;
		//cout << socket.recvLen << endl;
		++times;
		if(socket.recvLen < 0) {
			LOG("recv2 error.");
			break;
		}
		write(fd, upload.fileData, socket.recvLen);
		if(socket.recvLen < BUFFER_SIZE) {
			LOG("write finished.");
			break;
		} else {
			LOG("write success.");
		}
		//cout << "continue..." << endl;
		memset(upload.fileData, 0, sizeof(upload.fileData));
	}
	LOG("recv finished.");
	close(fd);

	return true;
}

int main() {
	Server server;
	Connection *conn;
    Statement *state;
    ResultSet *result;
	ConnPool *connpool = ConnPool::GetInstance();

	conn = connpool->GetConnection();
	state = conn->createStatement();
	string sql = "use ";
	sql += DB_NAME;
	state->execute(sql);
	
	if(server.ServerInit()) {
		LOG("server init success.");
	} else {
		LOG("server init failed.");
		return -1;
	}

	while(true) {
		if(server.ServerAccept()) {
			server.upload.UploadFile();
			server.upload.SaveToMysql(state);
			continue;
		} else {
			break;
		}
	}

	server.socket.SockClose();

	return 0;
}
