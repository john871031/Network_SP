#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <ctype.h>

#define MAXLEN 1024
int main(int argc, char **argv){
	int pid;
	int pfd1[2];
	int pfd2[2];
	char buf[MAXLEN];
	char input[] = {'I','n','p','u','t',':',' '};
	char output[] = {'O','u','t','p','u','t',':',' '};
	int len;
	int i;
	
	if(pipe(pfd1)==-1){
		perror("pipe1: ");
		exit(1);
	}
	if(pipe(pfd2)==-1){
		perror("pipe2: ");
		exit(1);
	}
	
	switch(pid = fork()){
		case -1:
			perror("fork: ");
			exit(1);
		
		default:		//parent
			close(pfd1[0]);		//先傳資料給child,所以第一個pipe不需要讀 
			close(pfd2[1]);		//輸出child轉成大寫的資料,所以第二個pipe不需要寫 
			if(write(STDOUT_FILENO, input, sizeof(input))<0){		//input:
					perror("write: ");
					exit(1);
				}
			while((len = read(STDIN_FILENO, buf, MAXLEN))>0){		//紀錄長度, 否則從buf讀資料會錯
				if(write(STDOUT_FILENO, output, sizeof(output))<0){	//output: 
					perror("write: ");
					exit(1);
				}
				if(write(pfd1[1], buf, len)<0){
					perror("write(pfd1): ");
					exit(1);
				}
				if((len = read(pfd2[0], buf, MAXLEN))<0){
					perror("read(pfd2): ");
					exit(1);
				}
				if(write(STDOUT_FILENO, buf, len)<0){
					perror("write: ");
					exit(1);
				}
				if(write(STDOUT_FILENO, input, sizeof(input))<0){	//input:
					perror("write: ");
					exit(1);
				}
				
			}		
			perror("read: ");
			exit(1);
			
		case 0:			//child
			close(pfd1[1]);		//與parent 相反 
			close(pfd2[0]);		//與parent 相反
			while((len = read(pfd1[0], buf, MAXLEN))>0){
				for(i=0; i<len; i++){		//小寫轉大寫
					buf[i] = toupper((unsigned char)buf[i]);
				}
				if(write(pfd2[1], buf, len)<0){
					perror("write(pfd2): ");
					exit(1);
				}
			}	
			perror("read(pfd1): ");
			exit(1);
	}	
	
	
	return 0;
}
