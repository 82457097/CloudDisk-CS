#include"Socket.h"

class Client {
public:

	File file;
	Socket socket;

	Client() {}
	~Client() {}

	bool ClientInit() {
		socket.SockInitClient();
		//socket.SockBind();
		file.fileData = (char*)malloc(BUFFER_SIZE);
		memset(file.fileData, '\0', sizeof(file.fileData));
		
		return true;
	}

	bool ClientSendTo() {
		unsigned int addrLen=sizeof(struct sockaddr);
	 
		cout << "please input file path: " << endl;
		cin >> file.filePath;
	 
		int fd = open(file.filePath, O_CREAT | O_RDWR, 0664);
		if(fd < 0) {
			printf("filepath not found!\n");
			return false;
		}
		
		printf("filepath : %s\n", file.filePath);
		int sendLen = socket.SockSendTo(file.filePath, 100, addrLen);
		if(sendLen < 0) {
			printf("filepath send error!\n");
			return false;
		} else {
			printf("filepath send success!\n");
		}

		printf("begin send data...\n");
		int times = 1;
		while((sendLen = read(fd, file.fileData, BUFFER_SIZE) > 0)) {
			printf("times = %d",times);
			times++;
			cout << sendLen << endl;
			if(socket.SockSendTo(file.fileData, sendLen, addrLen) < 0) {
				printf("send failed!\n");
				break;		
			} else {
				printf("send successful!\n");
			}
			if(sendLen < BUFFER_SIZE) {
				break;
			}
			memset(file.fileData, 0, BUFFER_SIZE); 
		}
		
		close(fd);
	
		return true;
	}

};
