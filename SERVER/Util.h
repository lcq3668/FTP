#ifndef UTIL_H
#define UTIL_H
#include<sys/socket.h>
int getFileName(int clientFd, char* fileName);
int consult(int clientFd, const char* buffer);
struct FD_PORT{
	int fd;
	int port;
};
void  buildChannel(int);
void setEmpty(struct FD_PORT * result);
void* wrapBuildChannel(void*);
                
void sendFile(int clientFd); // sock 是负责发送文件的
void sendAllFiles(int clientFd);
void receiveFile(int clientFd, int sock); // sock 的用处与 sendFile 相同
char  action(int clientFd);                                                        //  判断客户端是否要下载文件

#endif


