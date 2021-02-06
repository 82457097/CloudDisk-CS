#pragma once
#include <sys/epoll.h>
#include <unistd.h>  
#include <stdio.h>
#include <memory.h>
#include <errno.h>  
#include <iostream>
#include "Socket.h"

using namespace std;

#define MAX_EVENTS 500

class Epoll;
struct MyEvent     {  
    int fd;  
    void (*call_back)(Epoll, int, int, void*);  
    int events;  
    void *arg;  
    int status; 		// 1: in epoll wait list, 0 not in  
    char buff[128]; 	// recv data buffer  
    int len, s_offset;  
    long last_active; 	// last active time  
};

static int epollFd;
static MyEvent myEvents[MAX_EVENTS + 1];

class Epoll {
public:
	struct epoll_event evList[MAX_EVENTS];
	int epollFd;
	MyEvent myEvents[MAX_EVENTS + 1];

	Epoll() {
		epollFd = epoll_create(MAX_EVENTS);
	}
	~Epoll() {}
	
	void EventSet(MyEvent *ev, int fd, void (*call_back)(Epoll, int, int, void*), void *arg);

	void EventAdd(int epollFd, int events, MyEvent *ev);

	void EventDel(int epollFd, MyEvent *ev);

	bool SetNoBlock(int socket);

	static void AcceptConn(Epoll epoll, int fd, int events, void *arg);

	static void RecvData(Epoll epoll, int fd, int events, void *arg); 

	static void SendData(Epoll epoll, int fd, int events, void *arg);

	void EpollInit();
};
