#include"Server.h"

bool Server::ServerInit() {
	file.fileData = (char*)malloc(BUFFER_SIZE);
	memset(file.fileData, '\0', sizeof(file.fileData));

	return true;
}
	
/*bool Server::ServerAccept() {
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
}*/

/*bool Server::WriteFile(int recvfd, int fd) {
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
}*/

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
	Epoll epoll;
//	Connection *conn;
//    Statement *state;
//    ResultSet *result;
	
//	ConnPool *connpool = ConnPool::GetInstance();
//	conn = connpool->GetConnection();
//	state = conn->createStatement();
//	string sql = "use ";
//	sql += DB_NAME;
//	state->execute(sql);
	epoll.EpollInit();

	int checkPos = 0;  
	while(1) {	
		// a simple timeout check here, every time 100, better to use a mini-heap, and add timer event	
		long now = time(NULL);
	
		for(int i = 0; i < 100; i++, checkPos++) {	
			if(checkPos == MAX_EVENTS) {
				checkPos = 0;
			}
			
			if(epoll.myEvents[checkPos].status != 1) {
				continue;
			}
			
			long duration = now - epoll.myEvents[checkPos].last_active;
			if(duration >= 60) {
				LOG("fd: %d time out.", epoll.myEvents[checkPos].fd);
				//cout << epoll.myEvents[checkPos].fd << " time out" <<endl;
				Socket::SockClose(epoll.myEvents[checkPos].fd);
				printf("[fd=%d] timeout[%ld--%ld].\n", epoll.myEvents[checkPos].fd, epoll.myEvents[checkPos].last_active, now);  
				epoll.EventDel(epoll.epollFd, &(epoll.myEvents[checkPos]));  
			}  
		}  

		int fdNum = epoll_wait(epoll.epollFd, epoll.evList, MAX_EVENTS, -1);
		cout << fdNum << endl;
		if(fdNum < 0) {	
			cout << "epoll_wait error." << strerror(errno) << endl;
			break;	
		}  
		for(int i = 0; i < fdNum; i++) {	
			MyEvent *ev = (struct MyEvent*)epoll.evList[i].data.ptr;
			// read event
			if((epoll.evList[i].events&EPOLLIN) && (ev->events&EPOLLIN)) {	
				ev->call_back(&epoll, ev->fd, epoll.evList[i].events, ev->arg);  
			}
			// write event
			if((epoll.evList[i].events&EPOLLOUT) && (ev->events&EPOLLOUT)) {  
				ev->call_back(&epoll, ev->fd, epoll.evList[i].events, ev->arg);  
			}  
		}  
	}

//	connpool->DestoryConnPool();
	
	return 0;
}
