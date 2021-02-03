#include <sys/socket.h>  
#include <sys/epoll.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <stdio.h>
#include <memory.h>
#include <errno.h>  
#include <iostream>

using namespace std;

#define MAX_EVENTS 500

struct MyEvent     {  
    int fd;  
    void (*call_back)(int fd, int events, void *arg);  
    int events;  
    void *arg;  
    int status; 		// 1: in epoll wait list, 0 not in  
    char buff[128]; 	// recv data buffer  
    int len, s_offset;  
    long last_active; 	// last active time  
};

class Epoll {
public:
	Socket socket;
	struct epoll_event m_ev;
	struct epoll_event evList[MAX_EVENTS];
	int epollFd;
	MyEvent myEvents[MAX_EVENTS + 1];

	Epoll Epoll() {
		epollFd = epoll_create(MAX_EVENTS);
	}

	void EventSet(MyEvent *ev, int fd, void (*call_back)(int, int, void*), void *arg) {
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

	void EventAdd(int epollFd, int events, MyEvent *ev) {  
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


	bool SetNoBlock(int socket) {
		int opts = fcntl(Socket, F_GETFL);
		
		if(opts < 0) {
			cout << "fcntl(sock,GETFL)" << endl;
			return false;
		}
		
		opts = opts | O_NONBLOCK;
		if(fcntl(sock, F_SETFL, opts) < 0) {
			cout << "fcntl(sock,SETFL,opts)" << endl;
			return false;
		}

		return true;
	}

	void AcceptConn(int fd, int events, void *arg) {  
		int newFd = socket.SockAccpet();
		
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
	    if(SetNoBlock(socket.m_sockfd)) {
	        printf("%s: fcntl nonblocking failed.", __func__);
	        return ;
	    }
		
	    // add a read event for receive data  
	    EventSet(&myEvents[index], newFd, RecvData, &myEvents[index]);  
	    EventAdd(epollFd, EPOLLIN, &myEvents[index]);  
	 
	    printf("new conn[%s:%d][time:%d], pos[%d]\n", inet_ntoa(socket.m_sockaddr.sin_addr),
	            ntohs(socket.m_sockaddr.sin_port), myEvents[index].last_active, index);  
	} 

	void RecvData(int fd, int events, void *arg) {  
	    struct MyEvent *ev = static_cast<struct MyEvent*> arg;  
	    int len;
	    // receive data
	    len = socket.SockRecv()
	    len = recv(fd, ev->buff+ev->len, sizeof(ev->buff)-1-ev->len, 0);    
	    EventDel(g_epollFd, ev);
	    if(len > 0) {
	        ev->len += len;
	        ev->buff[len] = '\0';  
	        printf("C[%d]:%s\n", fd, ev->buff);  
	        // change to send event  
	        EventSet(ev, fd, SendData, ev);  
	        EventAdd(g_epollFd, EPOLLOUT, ev);  
	    } else if(len == 0) {  
	        close(ev->fd);  
	        printf("[fd=%d] pos[%d], closed gracefully.\n", fd, ev-g_Events);  
	    } else {  
	        close(ev->fd);  
	        printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));  
	    }  
	} 
	
};
