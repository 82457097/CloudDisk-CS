#include"Server.h"

/* 初始化客户连接，清空读缓冲区 */
void Server::init(int epollfd, int sockfd, const sockaddr_in& clientAddr) {
    file.fileData = (char*)malloc(BUFFER_SIZE);
	memset(file.fileData, '\0', sizeof(file.fileData));
    
	m_epollfd = epollfd;
    printf("m_sockfd:%d\n", sockfd);
	m_sockfd = sockfd;
	m_address = clientAddr;
}
	
void Server::process() {
    printf("Accept sock:%d\n", m_sockfd);
	int recvLen = Socket::SockRecv(m_sockfd, file.filePath, 100);
	if(recvLen < 0) {
		LOG("Recv error");
		removefd(m_epollfd, m_sockfd);
		return ;
	} else if(recvLen == 0) {
		LOG("客户端退出！");
		removefd(m_epollfd, m_sockfd);
		return ;
	} else {
		LOG("filepath is: %s", file.filePath);
		GetFileName();
	}
	LOG("filename: %s", file.fileName);
	
	int fd = open(file.fileName, O_CREAT | O_RDWR, 0664);
	if(fd < 0) {
		LOG("open file failed.");
	} else {
		WriteFile(fd);
	}
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

bool Server::WriteFile(int fd) {
	int times = 1;
	while(true) {
		int recvLen = Socket::SockRecv(m_sockfd, file.fileData, BUFFER_SIZE);
		++times;
		if(recvLen < 0) {
			LOG("Recv error");
			removefd(m_epollfd, m_sockfd);
			break;
		} else if(recvLen == 0) {
			LOG("客户端退出！");
			removefd(m_epollfd, m_sockfd);
			break;
		}
		
		write(fd, file.fileData, recvLen);
		if(recvLen < BUFFER_SIZE) {
			LOG("write finished.");
			break;
		} else {
			LOG("write success.");
		}
		memset(file.fileData, 0, sizeof(file.fileData));
	}
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

int main(int argc, char* argv[]) {
	if(argc <= 2) {
		printf("usage: %s ip_address port_number\n", basename(argv[0]));
		return 1;
	}
	const char* ip = argv[1];
	int port = atoi(argv[2]);
	int listenfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);
	int ret = 0;
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);
	ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
	assert(ret != -1);
	ret = listen(listenfd, 5);
	assert(ret != -1);

    /*Connection *conn;
    Statement *state;
    ResultSet *result;
    
    ConnPool *connpool = ConnPool::GetInstance();
    conn = connpool->GetConnection();
    state = conn->createStatement();
    string sql = "use ";
    sql += DB_NAME;
    state->execute(sql);*/

	ProcessPool<Server>* pool = ProcessPool<Server>::create(listenfd);
	if(pool) {
		pool->run();
		delete pool;
	}
	close(listenfd); /*正如前文提到的，main函数创建了文件描述符listenfd，那么就由它亲自关闭之*/
    //connpool->DestoryConnPool();
    
	return 0;
}


/*int main() {
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
}*/
