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
		cout << "sendPath failed." << endl;
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
		cout << "filepath : " << file.filePath << endl;
	}
	
	sendLen = socket.SockSend(socket.m_sockfd, file.filePath, 100);
	if(sendLen < 0) {
		cout << "filepath send error!" << endl;
		return false;
	} else {
		cout << "filepath send success!" << endl;
	}

	return true;
}

bool Client::SendFileData() {
	cout << "begin send data..." << endl;
	int times = 1;
	while((sendLen = read(fd, file.fileData, BUFFER_SIZE))) {
		if(sendLen < 0) {
			cout << "read error." << endl;
			break;
		}
		cout << "times = " << times << endl;
		++times;
		if(socket.SockSend(socket.m_sockfd, file.fileData, BUFFER_SIZE) < 0) {
			cout << "send failed!" << endl;
			break;		
		} else {
			cout << "send successful!" << endl;
		}
		if(sendLen < BUFFER_SIZE) {
			break;
		}
		memset(file.fileData, 0, BUFFER_SIZE); 
	}
	
	close(fd);
	return true;
}


int main() {
	Client client;

	if(client.ClientInit()) {
		cout << "client init success." << endl;
	} else {
		cout << "client init failed." << endl;
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

