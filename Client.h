#include"Socket.h"

class Client {
public:

	File file;
	Socket socket;
	int sendLen;
	int fd;

	Client() {}
	~Client() {}

	bool ClientInit();
	bool ClientSend();
	bool SendFilePath();
	bool SendFileData();

};
