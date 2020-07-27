#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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



int main(int argc, char** argv){
		char inputs[20];
		fgets(inputs,sizeof(inputs),stdin);
		parse(inputs,sizeof(inputs));
		ssize_t i = 0 ,end = sizeof(inputs)  ;
		for( ; i < end && strlen(inputs + i ) != 0;){
				printf("%s\n",inputs + i );
				i += strlen(inputs + i ) + 1;
				printf("i is %ld \n",i);
		}
		if(i < end){
				printf("zero\n");
		}
		return 0;
}
