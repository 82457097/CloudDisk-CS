#include"Socket.h"
#include"uploadfile.h"

class Server {
public:

	Socket socket;
	File file;
	Upload upload;

	Server() {}
	~Server() {}

	bool ServerInit();
	bool ServerAccept();
	bool GetFileName();
	bool WriteFile(int recvfd, int fd);
	
};
