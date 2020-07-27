#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<netinet/in.h>
#include<stdlib.h>
#include<errno.h>
#include<pthread.h>
#include"Util.h"
#define CONTROLERPORT 2020
#define QUEUESIZE 5
int main(int argc, char* argv[]){
    int socketControler, socketClient;
    struct sockaddr_in serverAddr, clientAddr;
    int addrLen;
    char * IP = "0.0.0.0";
    int clientFd;
    char fileName[101];
    int len;
    socketControler = socket(AF_INET,SOCK_STREAM,0);
    if(socketControler == -1){
        printf("Could not create socketControler\n");
    }
    serverAddr.sin_family = AF_INET;
    inet_aton(IP,&serverAddr.sin_addr); // To write your IP;
    serverAddr.sin_port = htons(CONTROLERPORT);
    if(bind(socketControler, (struct sockaddr *)&serverAddr, sizeof(serverAddr))<0){
        printf("Could not bind The ip : %s:%d\n",IP,CONTROLERPORT);
        
    }
    if(listen(socketControler,QUEUESIZE) != 0){
		printf("Could not set the soze of queue\n");
		// to use errorno
	}
    addrLen=  sizeof(clientAddr);
    while(1){
         clientFd = accept(socketControler,(struct sockaddr *)&clientAddr,(socklen_t*)&addrLen);
        if(clientFd<0){
            printf("Could not accept \n");
        }
        else{
            pthread_t thBuild;
            if(pthread_create(&thBuild,NULL,wrapBuildChannel,(void *) &clientFd)){
                perror("create thread");
            }
            //buildChannel(clientFd);
            if(pthread_detach(thBuild)){                    //  将线程分离出去 。用于只想线程自己执行完便自动清理和移除
                perror("detach");
            }
        }
        
    }
    
    
    return 0;
}
