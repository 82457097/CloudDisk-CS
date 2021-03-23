#include "MysqlConnPool.h"
#include "file.h"
#include "fastDFS.h"
#include "ProcessPool.h"
#include "Socket.h"

class Server {
public:
	File file;
	FastDFS fastDFS;

	Server() {}
	
	~Server() {}

	void init(int epollfd, int sockfd, const sockaddr_in& clientAddr);

    void process();
	
	bool GetFileName();
	
	bool WriteFile(int fd);
	
	bool UploadFile();
	
	bool SaveToMysql(Statement* state, string fileName, string fileId);

private:
    static int m_epollfd;
    int m_sockfd;
    sockaddr_in m_address;
    char m_buf[BUFFER_SIZE];
};

int Server::m_epollfd = -1;
