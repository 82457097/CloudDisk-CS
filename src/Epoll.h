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

	Epoll();
	
	void EventSet(MyEvent *ev, int fd, void (*call_back)(int, int, void*), void *arg);

	void EventAdd(int epollFd, int events, MyEvent *ev);

	void EventDel(int epollFd, MyEvent *ev);

	bool SetNoBlock(int socket);

	void AcceptConn(int fd, int events, void *arg);

	void RecvData(int fd, int events, void *arg); 

	void SendData(int fd, int events, void *arg);

	void EpollInit();
};
