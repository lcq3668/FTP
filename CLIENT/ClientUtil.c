#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include"ClientUtil.h"
int sendFileName(int fd,const char* flag,const char* fileName){              // flag  取值有两个：g , p 。前者意味着 get 下载; put 上传
	char len[104];
	int fileLen = strlen(fileName);
	strncpy(len,flag,strlen(flag));
	strncpy(len+strlen(flag),(char *)&fileLen,sizeofLen);
	strncpy(len + sizeofLen + strlen(flag),fileName,strlen(fileName));
	return write(fd,len,strlen(flag)+sizeofLen+ strlen(fileName));
}
int getPort(int fd){
	char port[4];
	read(fd,port,4);
	return *((int *)port);
}

void getFile(int socketClient,const char* fileName){
		 // To get the file size , then get the content of the target file
    unsigned long size;
	ssize_t n;
	char buffer[4096];
    read(socketClient,(char *) &size,8);
    printf("Client The target file size is %ld\n",size);
    int targetFd = open(fileName, O_WRONLY|O_CREAT ,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(targetFd == -1){
        printf("Client input file: %s\n",fileName);
        perror("Client open");   
    }
	ssize_t expectionSize;
    for(unsigned long offset = 0;offset < size; ){
		expectionSize = 4096<= size - offset ?4096:size - offset;
        n = read(socketClient,buffer,expectionSize);
        perror("Read");
        write(targetFd,buffer,n);
        offset += n;
    }
    //close(socketClient);
    close(targetFd);
	printf("Client : get the file  successfully\n");
}

void putFile(int socketClient, const char* fileName){
	    int targetFd = open(fileName,O_RDONLY,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        // To get the size of the file, then send the size to the client, next, send the data of file.
        struct stat st;
		fstat(targetFd,&st);
		printf("Client: The size of target file is %ld\n",st.st_size);
		char buffer[4096];
		strncpy(buffer,(char * )&st.st_size,8);
		write(socketClient,buffer,8);
		for(unsigned long offset = 0 ; offset < st.st_size;){
			ssize_t n = pread(targetFd,buffer,4096,offset);
			write(socketClient,buffer,n);
			offset += n;
		}
		close(targetFd);
		//close(socketClient);

		printf("put the file to the server successfully\n");
}

void translate(int socketClient){
		/* 从 下面开始, 搞成一个函数*/
		char inputs[2049];
		char action[4];
		char fileName[1025];
		while(1){
				fgets(inputs,sizeof(inputs),stdin);
				parse(inputs,sizeof(inputs));
				ssize_t i = 0 ,end = sizeof(inputs)  ;
				strncpy(action,inputs,strlen(inputs) +1 );
				i += strlen(inputs + i ) + 1;
				strncpy(fileName,inputs+i,strlen(inputs+i)+1);
				if(strlen(fileName) == 0){
						printf("Please input target fileNames. Do it again\n");
						continue;
				}
			    if(strcmp(action,"g") == 0){
						for( ; i < end && strlen(inputs + i ) != 0;){
								strncpy(fileName,inputs+i,strlen(inputs+i)+1);
								i += strlen(inputs + i ) + 1;
								sendFileName(socketClient,action,fileName);
								getFile(socketClient, fileName);
						}
				}
    			else if(strcmp(action,"p") == 0){
						for( ; i < end && strlen(inputs + i ) != 0;){
								strncpy(fileName,inputs+i,strlen(inputs+i)+1);
								i += strlen(inputs + i ) + 1;
								sendFileName(socketClient,action,fileName);
								putFile(socketClient, fileName);
						}
				}
				else if((strcmp(action,"exit") == 0){
						// close  sockfd  and exit;
				}
    			else{
					printf("Parameter is wrong. Please input 'g' or 'p' . Do it again! \n");
    			}
		}

}
void parse(char * inputs,unsigned long sizeInputs){
        /* checkExceedsion*/
		int flag =  checkExceedsion(inputs, sizeInputs);
		if(flag == 0){
				printf("exiting\n");
				exit(EXIT_FAILURE);
		}
		char * temp = (char*) malloc(sizeInputs);
		if(temp == NULL){
				perror("IN parse function : malloc\n");
				exit(EXIT_FAILURE);
		}

        /* remove BLank*/
        removeBlank(inputs);

		int counter = 0;
		int i= 0 ;
		for(ssize_t end = strlen(inputs);i<end;){
			while(i<end && *(inputs+i) != ' ' &&  *(inputs +i) != '\t'){

					temp[counter++] = inputs[i++]; 
			}
			if(i < end){
				temp[counter++] = '\0';

			}
			while(i<end && (inputs[i] == ' ' ||  inputs[i] == '\t')){
					i++;
			}
		}
		if(counter < sizeInputs){
				temp[counter++] = '\0';
		}
		memcpy(inputs,temp,counter);
		free(temp);	
}

void removeBlank(char * inputs){
        int i = 0;
		for(ssize_t end = strlen(inputs);i<end && (*(inputs+i) == ' ' ||  *(inputs +i) == '\t');i++) //  针对输入以空白字符开头的情况
				;        
		if(i != 0){
			strncpy(inputs,inputs + i,strlen(inputs + i )+1);
		}    
}

int checkExceedsion(char * inputs, unsigned long sizeInputs){
		if(strlen(inputs)<sizeInputs -1 ){
				inputs[strlen(inputs)-1] = '\0'; 
        }
        else if(strlen(inputs)==sizeInputs -1 ){
                if(inputs[sizeInputs -2]=='\n'){
                    inputs[sizeInputs -2]='\0';
                }
                else{
                    	while(getchar() != '\n');            // 继续清除多出的字符
                    	
						printf("inputs exceeded the limit : %ld\n",sizeInputs-2);
						return 0;
                }
        }
		return 1 ;
}
