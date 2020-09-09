#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<errno.h>
#include<memory.h> 
#include<stdlib.h>
#include"Client.h"

int main() {
	int sockcd;
	struct sockaddr_in server;
	int addrlen=sizeof(struct sockaddr);
	int fd;
	int lenpath;
	char *buffer;

	buffer = (char *)malloc(BUFFER_SIZE);
	memset(buffer, 0, BUFFER_SIZE); 
 
	if((sockcd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("socket build error!\n");
	}
	memset(&server, 0, sizeof(server));
	server.sin_family= AF_INET;
	server.sin_port = htons(8888);
	if(inet_pton(AF_INET, "127.0.0.1", &server.sin_addr)<0) {
		printf("inet_pton error!\n");
	}
 
	printf("file path:\n");
	scanf("%s", filePath);
 
	fd = open(filePath, O_CREAT | O_RDWR, 0664);
	if(fd < 0) {
		printf("filepath not found!\n");
		return 0;
 
	}
	printf("filepath : %s\n", filePath);
	lenpath = sendto(sockcd, filePath, strlen(filePath), 0, (struct sockaddr *)&server, addrlen);
	if(lenpath<0) {
		printf("filepath send error!\n");
	} else {
		printf("filepath send success!\n");
	}

	printf("begin send data...\n");
	int times = 1;
	while((fileTrans = read(fd, buffer, 1) > 0) {
		printf("times = %d",times);
		times++;
		if(sendto(sockcd, buffer, fileTrans, 0, (struct sockaddr *)&server, addrlen)<0) {
			printf("send failed!\n");
			break;		
		} else {
			printf("send successful!\n");
		}
		if(fileTrans < BUFFER_SIZE) {
			break;
		}
		memset(buffer, 0, BUFFER_SIZE); 
	}
	close(fd);
	close(sockcd);
	
	return 0;
}

