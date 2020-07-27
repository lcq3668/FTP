#include<arpa/inet.h>   //inet_addr
#include<unistd.h>      //write
#include<netinet/in.h>
#include<stdlib.h>
#include<errno.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include"ClientUtil.h"
int main(int argc ,char** argv){

    int  socketClient;
    struct sockaddr_in serverAddr, clientAddr;
    int addrLen;
    char * IP = "192.168.0.100";
    int clientFd;
    char fileName[100];
    int newPort = -1;
    const char * argvFileName =  argv[2];  
    const char * action = argv[1];
    if(argc <= 1){
        perror("argv");
        exit(EXIT_FAILURE);
    }
    socketClient = socket(AF_INET,SOCK_STREAM,0);
    
    if(socketClient == -1){
        printf("Could not create socketClient");
    }
    serverAddr.sin_family = AF_INET;
    inet_aton(IP,&serverAddr.sin_addr); // To write your IP;
    serverAddr.sin_port = htons(2020);
    if(connect(socketClient,(struct sockaddr *) &serverAddr,sizeof(struct sockaddr_in))){
        perror("connect");
    }
	/* to test the BigEndiumLittleEndium
    read(socketClient,fileName,4);
    strncpy((char*)&a,fileName,4);
    if(a == 112){
        printf("You are right\n");
    }
	*/
    newPort = getPort(socketClient);
    printf("The newPort is %d\n",newPort);
    close(socketClient);
    serverAddr.sin_port = htons(newPort);
    socketClient = socket(AF_INET,SOCK_STREAM,0);
    if(connect(socketClient,(struct sockaddr *) &serverAddr,sizeof(struct sockaddr_in))){
        perror("connect");
        exit(EXIT_FAILURE);
    }
    /*
    if(sendFileName(socketClient,action,argvFileName) != strlen(action) +  sizeofLen + strlen(argvFileName)){
	    perror("sendFileName wrong in len");
	    exit(EXIT_FAILURE);
    }
    */
    translate(action,socketClient,argvFileName);
    return 0;
}
