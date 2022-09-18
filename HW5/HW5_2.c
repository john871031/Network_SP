#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <wait.h>

#define MAXLEN 1024
#define MAXPFD 1024

int pfd_popen[MAXPFD] = {-1};

FILE *_popen(char *command, char *type){
	pid_t pid;
	int pfd[2];
	int fd; 
	
	if(strcmp(type, "r")!=0 && strcmp(type, "w")!=0){
		printf("請選擇popen方式('r' or 'w')\n");
		exit(1);
	}
	
	if(pipe(pfd) == -1){
		perror("pipe: ");
		exit(1);
	}
	
	switch(pid = fork()){
		case -1:
			perror("fork: ");
			exit(1);
		
		case 0:			//child
			switch(*type){
				case 'r':
					if(close(pfd[0]) < 0){
						perror("close(pfd[0]): ");
						exit(1);	
					}
					if(dup2(pfd[1], STDOUT_FILENO) < 0){
						perror("dup2(pfd[1]): ");
						exit(1);
					}
					if(close(pfd[1]) < 0){
						perror("close(pfd[1]): ");
						exit(1);
					}
					break;
				
				case 'w':
					if(close(pfd[1]) < 0){
						perror("close(pfd[1]): ");
						exit(1);	
					}
					if(dup2(pfd[0], STDIN_FILENO) < 0){
						perror("dup2(pfd[0]): ");
						exit(1);
					}
					if(close(pfd[0]) < 0){
						perror("close(pfd[0]): ");
						exit(1);
					}
					break;
					
			}
			execlp("/bin/sh", "sh", "-c", command, (char*) NULL);
			
		default:		//parent
			switch(*type){
				case 'r':
					if(close(pfd[1]) < 0){
						perror("close(pfd[1]): ");
						exit(1);	
					}
					fd = pfd[0];
					break;
				case 'w':
					if(close(pfd[0]) < 0){
						perror("close(pfd[0]): ");
						exit(1);	
					}
					fd = pfd[1];				
					break;	
			}
		pfd_popen[fd] = pid;		//記住取得的process id ,為了_pclose能選擇要等待的process
		return fdopen(fd, type);	//將fd轉為FILE的位址
	}
}
int _pclose(FILE *file){
	int fd;
	pid_t pid;
	int status;
	
	fd = fileno(file);		//將file轉為檔案描述符
	if(close(fd) < 0){
		perror("close(fd): ");
		exit(1);
	}
	fflush(file);
	pid = pfd_popen[fd];
	if (waitpid(pid, &status, 0) < 0){	//等待所選的process
		return -1;		
	}
	return pid;
}
int main(int argc, char *argv[]){
	FILE *file;
	char buf[MAXLEN];
	int len;
	
	if(argc != 3){
		printf("輸入格式錯誤");
		exit(1);
	}
	
	if((file = _popen(argv[1], argv[2]))==NULL){		//先輸入command, 接著輸入type('r' or 'w')
		perror("_popen: ");
		exit(1);
	}
	if(strcmp(argv[2], "r")==0){
		fread(buf, MAXLEN, MAXLEN, file);
		printf("%s", buf);
	}else if(strcmp(argv[2], "w")==0){
		fgets(buf, MAXLEN, stdin);
		len = strlen(buf);
		if(buf[len-1] == '\n'){		//確保裡面是有效字符 
			buf[len-1] == '\0';
		}
		printf("%s", buf);
	}
	if(_pclose(file) < 0){
		perror("_pclose: ");
		exit(1);
	} 
	
	return 0;
} 
