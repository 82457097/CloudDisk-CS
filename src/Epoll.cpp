#include "Epoll.h"

Epoll::Epoll() {
	epollFd = epoll_create(MAX_EVENTS);
}

void Epoll::EventSet(MyEvent *ev, int fd, void (*call_back)(Epoll, int, int, void*), void *arg) {
	ev->fd = fd;
    ev->call_back = call_back;
    ev->events = 0;
    ev->arg = arg;
    ev->status = 0;
    memset(ev->buff, sizeof(ev->buff), 0);
    ev->s_offset = 0;  
    ev->len = 0;
    ev->last_active = time(NULL);
	cout << "listen fd: " << ev->fd <<endl;
}

void Epoll::EventAdd(int epollFd, int events, MyEvent *ev) {
	cout << "epollFd: " <<epollFd <<endl;
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
	
	if (epoll_ctl(epollFd, oprate, ev->fd, &epv) < 0) {
		printf("Event Add failed[fd=%d], evnets[%d]\n", ev->fd, events);
	} else {  
		printf("Event Add OK[fd=%d], op=%d, evnets[%0X]\n", ev->fd, oprate, events);
		cout << "Event Add OK" << endl;
	}
}

void Epoll::EventDel(int epollFd, MyEvent *ev) {  
	struct epoll_event epv = {0, {0}};	
	if(ev->status != 1) {
		return;
	}
	epv.data.ptr = &(ev->fd);	
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

void Epoll::AcceptConn(Epoll epoll, int fd, int events, void *arg) {
	struct sockaddr_in sin;  
    socklen_t len = sizeof(struct sockaddr_in); 
	int newFd = Socket::SockAccpet(fd, sin);
	
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
    if(epoll.SetNoBlock(fd)) {
        printf("%s: fcntl nonblocking failed.", __func__);
        return ;
    }
	struct myevent_s *ev = (struct myevent_s*)arg;
    // add a read event for receive data  
    epoll.EventSet(&myEvents[index], newFd, RecvData, &myEvents[index]);  
    epoll.EventAdd(epollFd, EPOLLIN, &myEvents[index]);  
 
    printf("new conn[%s:%d][time:%d], pos[%d]\n", inet_ntoa(sin.sin_addr),
		ntohs(sin.sin_port), myEvents[index].last_active, index);  
} 

void Epoll::RecvData(Epoll epoll, int fd, int events, void *arg) {  
    struct MyEvent *ev = static_cast<struct MyEvent*>(arg);  
    int len;
    // receive data
    len = Socket::SockRecv(fd, ev->buff+ev->len, sizeof(ev->buff)-1-ev->len);   
    epoll.EventDel(epollFd, ev);
    if(len > 0) {
        ev->len += len;
        ev->buff[len] = '\0';  
        printf("C[%d]:%s\n", fd, ev->buff);  
        // change to send event  
        epoll.EventSet(ev, fd, SendData, ev);  
        epoll.EventAdd(epollFd, EPOLLOUT, ev);  
    } else if(len == 0) {  
        Socket::SockClose(ev->fd); 
        printf("[fd=%d] pos[%d], closed gracefully.\n", fd, ev-epollFd);  
    } else {  
        Socket::SockClose(ev->fd);  
        printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));  
    }  
} 

void Epoll::SendData(Epoll epoll, int fd, int events, void *arg) {  
    struct MyEvent *ev = static_cast<struct MyEvent*>(arg);  
    int len;  
    // send data
    len = Socket::SockSend(fd, ev->buff + ev->s_offset, ev->len - ev->s_offset);
    if(len > 0) {
        printf("send[fd=%d], [%d<->%d]%s\n", fd, len, ev->len, ev->buff);
        ev->s_offset += len;
        if(ev->s_offset == ev->len) {
            // change to receive event
            epoll.EventDel(epollFd, ev);  
            epoll.EventSet(ev, fd, RecvData, ev);  
            epoll.EventAdd(epollFd, EPOLLIN, ev);  
        }
    } else {  
        Socket::SockClose(ev->fd);
        epoll.EventDel(epollFd, ev);  
        printf("send[fd=%d] error[%d]\n", fd, errno);  
    }  
}

void Epoll::EpollInit() {
	Socket socket;
	SetNoBlock(socket.m_sockfd);
	cout << "Server listen fd: " << socket.m_sockfd << endl;
    // bind & listen  
	socket.SockInitServer();
	socket.SockBind(); 
    socket.SockListen();
	EventSet(&myEvents[MAX_EVENTS], socket.m_sockfd, AcceptConn, &myEvents[MAX_EVENTS]);  
    // add listen socket  
    EventAdd(epollFd, EPOLLIN, &myEvents[MAX_EVENTS]);
}

