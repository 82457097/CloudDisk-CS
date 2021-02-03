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
	socket.SockRecv(recvfd, file.filePath, 100);
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
		memset(file.fileData, 0, sizeof(file.fileData));
	}
	LOG("recv finished.");
	close(fd);

	return true;
}

bool Server::UploadFile() {
	if (!fastDFS.FdfsClientInit()) {
		LOG("fastdfs initial failed.");
		return false;
	}

	if (!fastDFS.TrackerGetConnection()) {
		LOG("tracker initial failed.");
		fastDFS.FdfsClientDestroy();
		return false;
	}

	if (!fastDFS.TrackerQueryStorageStore()) {
		LOG("storage initial failed.");
		fastDFS.FdfsClientDestroy();
		return false;
	}

	if (!fastDFS.StorageUploadByFilename1(file.fileName, file.fileId)) {
		LOG("StorageUploadByFilename1 initial failed.");
		fastDFS.FdfsClientDestroy();
		return false;
	}

	if (!fastDFS.TrackerCloseConnectionEx()) {
		LOG("TrackerCloseConnectionEx initial failed.");
		fastDFS.FdfsClientDestroy();
		return false;
	}

	fastDFS.FdfsClientDestroy();
	
	return true;
}

bool Server::SaveToMysql(Statement* state, string fileName, string fileId) {
	char sql[SQL_LEN] = { '\0' };
	snprintf(sql, SQL_LEN, "insert into %s values(NULL, '%s', '%s')", TABLE_NAME, fileName.c_str(), fileId.c_str());

	if (state->executeUpdate(sql)) {
		return true;
	}

	return false;
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
		server.socket.SockClose();
		connpool->DestoryConnPool();
		return -1;
	}

	while(true) {
		if(server.ServerAccept()) {
			server.UploadFile();
			server.SaveToMysql(state, server.file.fileName, server.file.fileId);
			continue;
		} else {
			break;
		}

		server.socket.SockClose();
		connpool->DestoryConnPool();
		return -1;
	}

	server.socket.SockClose();
	connpool->DestoryConnPool();
	
	return 0;
}
