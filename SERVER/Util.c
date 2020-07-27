#include"Util.h"
#include<fcntl.h>
#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<netinet/in.h>
#include<stdlib.h>
#include <errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pthread.h>
#define QUEUESIZE 5
int getFileName(int clientFd,char* FileName){
    /*The size of FileName must not be less than 100*/
    read(clientFd,FileName,4);
    int len = *(int *)FileName;
    read(clientFd,FileName,len);
    return len;
}
int consult(int clientFd, const char* buffer){
    return write(clientFd,buffer,4);
}
void setEmpty(struct FD_PORT * result){
        result->port = -1;
        result->fd = -1;
}  
void  buildChannel(int clientFd){
        /* 返回可用的端口和新的控制socket fd*/
        const char * IP = "0.0.0.0";
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in serv_addr;
        int freePort;
        char port[4];
        //struct FD_PORT result;
        if(sock < 0) {
                printf("socket error\n");
                //setEmpty(&result);
                return  ;
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        inet_aton(IP,&serv_addr.sin_addr);
        serv_addr.sin_port = 0;
        if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            if(errno == EADDRINUSE) {
                printf("the port is not available. already to other process\n");
                        //setEmpty(
                return ;
            }
            else {
                printf("could not bind to process (%d) %s\n", errno, strerror(errno));
                return ;
            }
        }
        socklen_t len = sizeof(serv_addr);
        if (getsockname(sock, (struct sockaddr *)&serv_addr, &len) == -1) {
                perror("getsockname");
                //setEmpty(&result);
                return ;
        }
        if(listen(sock,QUEUESIZE) != 0){
            printf("Could not set the soze of queue\n");
                // to use errorno
        }
        freePort = ntohs(serv_addr.sin_port);                // 随机地 生成一个端口
        //printf("port number %d\n", freePort);
        //result.port = freePort;
        //result.fd = sock;
        strncpy(port,(char *)& freePort,4);                      
        if(consult(clientFd,port) != 4 ){                              //  向客户端发送端口信息
            perror("consult. The number received is not 4!");
        }
        else{
            printf("consult successfully!\n");
        }
        close(clientFd);                                                            //  关闭关于原端口的文件描述符。
        /*To wait serval second?*/
        
        if((clientFd = accept(sock,(struct sockaddr *)&serv_addr,(socklen_t*)&len)) <= 0){
            perror("accept");
        }
        else{
            while(1){
            char flag = action(clientFd);
            if(flag == 'g')
                // sendFile(clientFd,sock);
                sendAllFiles(client);
            else if(flag == 'p'){ 
                receiveFile(clientFd,sock);                   

            }
            }

                
            

        }
}

void* wrapBuildChannel(void* argsBuffer){
    int clientFd = *(int *)argsBuffer;
    buildChannel(clientFd);
    pthread_exit(NULL);
}

void sendFile(int clientFd){
            char fileName[101];
            int targetFd ;
            int len;
            if(( len = getFileName(clientFd, fileName))>0){
                fileName[len] = '\0';
                printf("length of file name is: %d\n",len);
                printf("Server:The file name is: %s\n",fileName);
                targetFd = open(fileName,O_RDONLY,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                // To get the size of the file, then send the size to the client, next, send the data of file.
                struct stat st;
                fstat(targetFd,&st);
                printf("Server: The size of target file is %ld\n",st.st_size);
                strncpy(fileName,(char * )&st.st_size,8);
                write(clientFd,fileName,8);
                char buffer[4096];
                for(unsigned long offset = 0 ; offset < st.st_size;){
                    ssize_t n = pread(targetFd,buffer,4096,offset);
                    write(clientFd,buffer,n);
                    offset += n;
                }
                
                close(targetFd);
                close(clientFd);
                //close(sock);
                printf("send the file to the client successfully\n");
            }
}

void sendAllFiles(int clientFd){
        char fileName[1025] ;
        int targetFd ;
        int len;
        if(( len = getFileName(clientFd, fileName)) <= 0){
                perror("sendAllFiles");
                return ;
        }
        // 判断是否文件。

}

void receiveFile(int clientFd,int sock){
        char fileName[101];
        int targetFd ;
        int len;
        if(( len = getFileName(clientFd, fileName))>0){
            fileName[len] = '\0';
            printf("length of file name is: %d\n",len);
            printf("Server:The file name is: %s\n",fileName);
            targetFd = open(fileName, O_WRONLY|O_CREAT ,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            // To get the size of the file, then send the size to the client, next, send the data of file.
            if(targetFd == -1){
                perror("server  open file in reading");   
                return;
            }
            unsigned long fileSize;
            read(clientFd,(char*)&fileSize,8);                                                     // 此时 用 fileName 存储文件大小
            ssize_t  n ;
            char buffer[4096];
            for(unsigned long offset = 0 ; offset < fileSize;){
                n = read(clientFd,buffer,4096);
                perror("Read");
                write(targetFd,buffer,n);
                offset += n;
            }        
            close(targetFd);
            close(clientFd);
            close(sock);
            printf("get the file to the client successfully\n");
            }
}



char action(int clientFd){
        char flag;
        read(clientFd,&flag,1);
        return flag;
        
}
