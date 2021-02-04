#include "Epoll.h"

Epoll::Epoll() {
	epollFd = epoll_create(MAX_EVENTS);
}

void Epoll::EventSet(MyEvent *ev, int fd, void (*call_back)(int, int, void*), void *arg) {
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

void Epoll::EventAdd(int epollFd, int events, MyEvent *ev) {  
	struct epoll_event epv = {0, {0}};	
	int oprate;  
	epv.data.ptr = ev->fd;	
	epv.events = ev->events = events;  
	if(ev->status == 1) {  
		oprate = EPOLL_CTL_MOD;  
	} else {  
		oprate = EPOLL_CTL_ADD;  
		ev->status = 1;  
	}
	
	if (epoll_ctl(epollFd, oprate, ev->fd, &epv) < 0) {
		printf("Event Add failed[fd=%d], evnets[%d]\n", ev->fd, events);
	} else {  
		printf("Event Add OK[fd=%d], op=%d, evnets[%0X]\n", ev->fd, oprate, events);  
	}
}

void EventDel(int epollFd, MyEvent *ev) {  
	struct epoll_event epv = {0, {0}};	
	if(ev->status != 1) {
		return;
	}
	epv.data.ptr = ev->fd;	
	ev->status = 0;
	epoll_ctl(epollFd, EPOLL_CTL_DEL, ev->fd, &epv);  
}

bool Epoll::SetNoBlock(int fd) {
	int opts = fcntl(fd, F_GETFL);
	
	if(opts < 0) {
		cout << "fcntl(sock,GETFL)" << endl;
		return false;
	}
	
	opts = opts | O_NONBLOCK;
	if(fcntl(fd, F_SETFL, opts) < 0) {
		cout << "fcntl(sock,SETFL,opts)" << endl;
		return false;
	}

	return true;
}

void Epoll::AcceptConn(int fd, int events, void *arg) {  
	int newFd = Socket::SockAccpet();
	
 	int index;
    for(index = 0; index < MAX_EVENTS; index++) {  
        if(myEvents[index].status == 0) {  
            break;  
        }  
    }
	
    if(index == MAX_EVENTS) {  
        printf("%s:max connection limit[%d].", __func__, MAX_EVENTS);  
        return ; 
    }
	
    // set nonblocking
    if(SetNoBlock(fd)) {
        printf("%s: fcntl nonblocking failed.", __func__);
        return ;
    }
	
    // add a read event for receive data  
    EventSet(&myEvents[index], newFd, RecvData, &myEvents[index]);  
    EventAdd(epollFd, EPOLLIN, &myEvents[index]);  
 
    //printf("new conn[%s:%d][time:%d], pos[%d]\n", inet_ntoa(socket.m_sockaddr.sin_addr),
    //       ntohs(socket.m_sockaddr.sin_port), myEvents[index].last_active, index);  
} 

void Epoll::RecvData(int fd, int events, void *arg) {  
    struct MyEvent *ev = static_cast<struct MyEvent*> arg;  
    int len;
    // receive data
    len = Socket::SockRecv(fd, ev->buff+ev->len, sizeof(ev->buff)-1-ev->len);   
    EventDel(epollFd, ev);
    if(len > 0) {
        ev->len += len;
        ev->buff[len] = '\0';  
        printf("C[%d]:%s\n", fd, ev->buff);  
        // change to send event  
        EventSet(ev, fd, SendData, ev);  
        EventAdd(epollFd, EPOLLOUT, ev);  
    } else if(len == 0) {  
        Socket::SockClose(ev->fd); 
        printf("[fd=%d] pos[%d], closed gracefully.\n", fd, ev-epollFd);  
    } else {  
        Socket::SockClose(ev->fd);  
        printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));  
    }  
} 

void Epoll::SendData(int fd, int events, void *arg) {  
    struct MyEvent *ev = static_cast<struct MyEvent*> arg;  
    int len;  
    // send data
    len = Socket::SockSend(fd, ev->buff + ev->s_offset, ev->len - ev->s_offset);
    if(len > 0) {
        printf("send[fd=%d], [%d<->%d]%s\n", fd, len, ev->len, ev->buff);
        ev->s_offset += len;
        if(ev->s_offset == ev->len) {
            // change to receive event
            EventDel(epollFd, ev);  
            EventSet(ev, fd, RecvData, ev);  
            EventAdd(epollFd, EPOLLIN, ev);  
        }
    } else {  
        Socket::SockClose(ev->fd);
        EventDel(epollFd, ev);  
        printf("send[fd=%d] error[%d]\n", fd, errno);  
    }  
}

void Epoll::EpollInit(Socket socket) {  
	SetNoBlock(socket.m_sockfd) 
	cout << "Server listen fd: " << socket.m_sockfd << endl;
    // bind & listen  
	Socket::SockInitServer();
	Socket::SockBind(); 
    Socket::SockListen();
	EventSet(&myEvents[MAX_EVENTS], socket.m_sockfd, AcceptConn, &myEvents[MAX_EVENTS]);  
    // add listen socket  
    EventAdd(epollFd, EPOLLIN, &myEvents[MAX_EVENTS]);
}

