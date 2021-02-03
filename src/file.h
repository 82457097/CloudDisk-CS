#pragma once
#include<string>
#include"fastCGI.h"

using namespace std;

#define FILE_NAME_LEN 512
#define FILE_PATH_LEN 512
#define FILE_ID_LEN 512

class File {
public:
	char fileName[FILE_NAME_LEN] = { '\0' };
	char filePath[FILE_PATH_LEN] = { '\0' };
	char fileId[FILE_ID_LEN] = { '\0' };
	char *fileData;
	char *pbegin = nullptr, *pend = nullptr, *ptemp = nullptr;
	char *contentLen;
	FastCGI fastCGI;
	
public:
	File() {}
	~File() {}
};
