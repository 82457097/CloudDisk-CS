#include "Epoll.h"

void Epoll::EventSet(MyEvent *ev, int fd, void (*call_back)(Epoll*, int, int, void*), void *arg) {
	ev->fd = fd;
    ev->call_back = call_back;
    ev->events = 0;
    ev->arg = arg;
    ev->status = 0;
    memset(ev->buff, sizeof(ev->buff), 0);
    ev->s_offset = 0;  
    ev->len = 0;
    ev->last_active = time(NULL);
}

void Epoll::EventAdd(int epollfd, int events, MyEvent *ev) {
	struct epoll_event epv = {0, {0}};	
	int oprate;  
	epv.data.ptr = &(ev->fd);	
	epv.events = ev->events = events;  
	if(ev->status == 1) {  
		oprate = EPOLL_CTL_MOD;  
	} else {  
		oprate = EPOLL_CTL_ADD;  
		ev->status = 1;  
	}
	
	if (epoll_ctl(epollfd, oprate, ev->fd, &epv) < 0) {
		printf("Event Add failed[fd=%d], evnets[%d]\n", ev->fd, events);
	} else {  
		printf("Event Add OK[fd=%d], op=%d, evnets[%0X]\n", ev->fd, oprate, events);
	}
}

void Epoll::EventDel(int epollfd, MyEvent *ev) {  
	struct epoll_event epv = {0, {0}};	
	if(ev->status != 1) {
		return;
	}
	epv.data.ptr = &(ev->fd);	
	ev->status = 0;
	epoll_ctl(epollfd, EPOLL_CTL_DEL, ev->fd, &epv);  
}

bool Epoll::SetNoBlock(int fd) {
	int opts = fcntl(fd, F_GETFL);
	
	if(opts < 0) {
		cout << "fcntl(sock, GETFL)" << endl;
		return false;
	}
	
	opts = opts | O_NONBLOCK;
	if(fcntl(fd, F_SETFL, opts) < 0) {
		cout << "fcntl(sock, SETFL, opts)" << endl;
		return false;
	}

	return true;
}

void Epoll::AcceptConn(Epoll *epoll, int fd, int events, void *arg) {
	struct sockaddr_in sin;  
	int newFd = Socket::SockAccpet(fd, sin);
	LOG("new fd: %d.", newFd);

	int index;
    for(index = 0; index < MAX_EVENTS; index++) {  
        if(epoll->myEvents[index].status == 0) {
			cout << "index: " << index <<endl;
            break;  
        }  
    }
	
    if(index == MAX_EVENTS) {  
		LOG("max connection limit[%d].", MAX_EVENTS);
        return ; 
    }
	
    // set nonblocking
    if(epoll->SetNoBlock(newFd)) {
		LOG("fcntl nonblocking success.");
    }

    // add a read event for receive data  
    epoll->EventSet(&(epoll->myEvents[index]), newFd, RecvData, &(epoll->myEvents[index]));  
    epoll->EventAdd(epoll->epollFd, EPOLLIN, &(epoll->myEvents[index]));  
 
    printf("new conn[%s:%d][time:%ld], pos[%d]\n", inet_ntoa(sin.sin_addr),
		ntohs(sin.sin_port), epoll->myEvents[index].last_active, index);  
} 

void Epoll::RecvData(Epoll *epoll, int fd, int events, void *arg) {  
    struct MyEvent *ev = static_cast<struct MyEvent*>(arg);
	int recvLen = Socket::SockRecv(fd, ev->buff, BUFFER_SIZE);
	cout << recvLen << endl;
	epoll->EventDel(epoll->epollFd, ev);
	
	if(recvLen > 0) {
		if(ev->buff[0] == '/' && ev->fileName == nullptr) {
			ev->fileName = (char*)malloc(512);
			epoll->GetFileName(ev->fileName, ev->buff);
			LOG("filename: %s", ev->fileName);
		}

		if(ev->fileName != nullptr) {
			int ffd = open(ev->fileName, O_CREAT | O_RDWR, 0664);
			if(ffd < 0) {
				LOG("open file failed.");
				return ;
			} else {
				write(fd, ev->buff, recvLen);
				memset(ev->buff, 0, sizeof(ev->buff));
				if(recvLen < BUFFER_SIZE) {
					LOG("write finished.");
				} else {
					epoll->WriteFile(fd, ffd, ev->buff);
				}
			}
		}
	} else if(recvLen == 0) {
		Socket::SockClose(ev->fd);
		printf("[fd=%d] pos[%d], closed gracefully.\n", fd, epoll->epollFd);
	} else {  
        Socket::SockClose(ev->fd);  
        printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));  
    }
		
	epoll->EventSet(ev, fd, RecvData, ev);	
	epoll->EventAdd(epoll->epollFd, EPOLLIN, ev);	
} 

void Epoll::SendData(Epoll *epoll, int fd, int events, void *arg) {  
    struct MyEvent *ev = static_cast<struct MyEvent*>(arg);  
    int len = Socket::SockSend(fd, ev->buff + ev->s_offset, ev->len - ev->s_offset);
    if(len > 0) {
        printf("send[fd=%d], [%d<->%d]%s\n", fd, len, ev->len, ev->buff);
        ev->s_offset += len;
        if(ev->s_offset == ev->len) {
            // change to receive event
            epoll->EventDel(epoll->epollFd, ev);  
            epoll->EventSet(ev, fd, RecvData, ev);  
            epoll->EventAdd(epoll->epollFd, EPOLLIN, ev);  
        }
    } else {  
        Socket::SockClose(ev->fd);
        epoll->EventDel(epoll->epollFd, ev);  
        printf("send[fd=%d] error[%d]\n", fd, errno);  
    }  
}

void Epoll::EpollInit() {
	Socket socket;
	SetNoBlock(socket.m_sockfd);
	// bind & listen  
	socket.SockInitServer();
	socket.SockBind(); 
    socket.SockListen();
	cout << "Server listen fd: " << socket.m_sockfd << endl;
	EventSet(this->myEvents, socket.m_sockfd, AcceptConn, this->myEvents);  
    // add listen socket  
    EventAdd(this->epollFd, EPOLLIN, this->myEvents);
}

void Epoll::GetFileName(char* fileName, char* filePath) {
	int i = 0, k = 0;
	for(i = strlen(filePath); i > 0; --i) {
		if(filePath[i] != '/') {
			++k;
		} else {
			break;
		}
	}

	strcpy(fileName, filePath + (strlen(filePath) - k) + 1);
}

void Epoll::WriteFile(int recvfd, int fd, char* buff) {
	int times = 1;
	int recvLen = 0;
	while(recvLen = Socket::SockRecv(recvfd, buff, BUFFER_SIZE)) {
		++times;
		if(recvLen < 0) {
			LOG("recv2 error.");
			break;
		}
		write(fd, buff, recvLen);
		if(recvLen < BUFFER_SIZE) {
			LOG("write finished.");
			break;
		} else {
			LOG("write success.");
		}
		memset(buff, 0, sizeof(buff));
	}
	LOG("recv finished.");
}




