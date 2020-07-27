#ifndef CLIENTUTIL_H
#define CLIENTUTIL_H
#define sizeofLen 4             // 表示用 4 个 字节描述文件名的长度
int sendFileName(int fd,const char* flag,const char* fileName);
int getPort(int fd);
void getFile(int socketClient,const char* fileName);
void putFile(int socketClient, const char* fileName);
void translate(int socketCLient);
struct Action{
        char* operation;
        char** fileNames;
        int amountFiles;
};
typedef struct Action Action;

void  parse(char * inputs,unsigned long sizeInputs);  
void removeBlank(char * inputs);
int checkExceedsion(char * inputs, unsigned long sizeInputs)
#endif

