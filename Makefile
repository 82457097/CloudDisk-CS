
All:
	g++ -std=c++11 -o Client file.h Client.h Client.cpp Socket.h Socket.cpp
	g++ -std=c++11 -o Server Server.h Server.cpp Socket.h Socket.cpp file.h

Client:
	g++ -std=c++11 -o Client file.h Client.h Client.cpp Socket.h Socket.cpp

Server:
	g++ -std=c++11 -o Server Server.h Server.cpp Socket.h Socket.cpp file.h

.PHONY: Clean
Clean:
	rm Client Server
