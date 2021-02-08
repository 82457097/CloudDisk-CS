#include"Client.h"

bool Client::ClientInit() {
	socket.SockInitClient();
	socket.SockConnect();
	file.fileData = (char*)malloc(BUFFER_SIZE);
	memset(file.fileData, '\0', sizeof(file.fileData));
	
	return true;
}

bool Client::ClientSend() { 		
	if(!SendFilePath()) {
		LOG("sendPath failed.");
		return false;
	}
	
	SendFileData();
	
	return true;
}

bool Client::SendFilePath() {
	cout << "please input file path: " << endl;
	cin >> file.filePath;
 	
	fd = open(file.filePath, O_RDWR, 0664);
	if(fd < 0) {
		cout << "filepath not found!" << endl;
		return false;
	} else {
		LOG("filepath : %s", file.filePath);
	}

	sendLen = socket.SockSend(socket.m_sockfd, file.filePath, 100);
	if(sendLen < 0) {
		LOG("filepath send error!");
		return false;
	} else {
		LOG("filepath send success!");
	}

	return true;
}

bool Client::SendFileData() {
	cout << "begin send data..." << endl;
	int times = 1;
	while((sendLen = read(fd, file.fileData, BUFFER_SIZE))) {
		if(sendLen < 0) {
			LOG("read error.");
			break;
		} else if(sendLen == BUFFER_SIZE) {
			cout << "times = " << times << endl;
			++times;
			if(socket.SockSend(socket.m_sockfd, file.fileData, BUFFER_SIZE) < 0) {
				LOG("send failed!");
				break;		
			} else {
				LOG("data len is: %d", sendLen);
				LOG("send successful!");
			}
		} else if(sendLen < BUFFER_SIZE) {
			cout << "times = " << times << endl;
			++times;
			if(socket.SockSend(socket.m_sockfd, file.fileData, sendLen) < 0) {
				LOG("send failed!");
				break;		
			} else {
				LOG("data len is: %d", sendLen);
				LOG("send successful!");
			}
			break;
		}
		memset(file.fileData, 0, BUFFER_SIZE); 
	}
	
	close(fd);
	return true;
}


int main() {
	Client client;
	Socket socket;
	
	if(client.ClientInit()) {
		LOG("client init success.");
	} else {
		LOG("client init failed.");
	}

	while(true) {
		if(client.ClientSend()) {
			continue;
		} else {
			break;
		}
	}

	client.socket.SockClose();
	
	return 0;
}

