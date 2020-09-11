#include"Server.h"

int main() {
	Server server;
	
	if(server.ServerInit()) {
		cout << "server init success." << endl;
	} else {
		cout << "server init failed." << endl;
		return -1;
	}


	while(true) {
		if(server.ServerRecv()) {
			continue;
		} else {
			break;
		}
	}

	server.socket.SockClose();

	return 0;
}
