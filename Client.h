#include"Socket.h"

class Client {
public:

	File file;
	Socket socket;
	int sendLen;
	int fd;

	Client() {}
	~Client() {}

	bool ClientInit() {
		socket.SockInitClient();
		socket.SockConnect();
		file.fileData = (char*)malloc(BUFFER_SIZE);
		memset(file.fileData, '\0', sizeof(file.fileData));
		
		return true;
	}

	bool ClientSend() { 		
		SendFilePath();
		SendFileData();
		
		return true;
	}

	bool SendFilePath() {
		cout << "please input file path: " << endl;
		cin >> file.filePath;
	 
		fd = open(file.filePath, O_RDWR, 0664);
		if(fd < 0) {
			printf("filepath not found!\n");
			return false;
		} else {
			printf("filepath : %s\n", file.filePath);
		}
		
		sendLen = socket.SockSend(socket.m_sockfd, file.filePath, 100);
		if(sendLen < 0) {
			printf("filepath send error!\n");
			return false;
		} else {
			printf("filepath send success!\n");
		}

		return true;
	}

	bool SendFileData() {
		printf("begin send data...\n");
		int times = 1;
		while((sendLen = read(fd, file.fileData, BUFFER_SIZE))) {
			if(sendLen < 0) {
				cout << "read error." << endl;
				break;
			}
			printf("times = %d\n", times);
			++times;
			if(socket.SockSend(socket.m_sockfd, file.fileData, BUFFER_SIZE) < 0) {
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
