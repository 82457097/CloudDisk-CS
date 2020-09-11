
Client:
	g++ -std=c++11 -o Client file.h Client.h Client.cpp Socket.h Socket.cpp

Server:
	g++ -std=c++11 -o Server Server.h Server.cpp Socket.h Socket.cpp file.h

Clean:
	rm Client Server
