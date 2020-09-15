#include"Client.h"

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

