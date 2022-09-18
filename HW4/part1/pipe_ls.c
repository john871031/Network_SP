#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	
	pid_t pid;
	int p[2];
	char *command[] = {"ls", "-l", NULL};
	char buf[1024];
	
	if(pipe(p)<0){
		return 1;
	}
	
	switch(pid = fork()){
		
		case -1:		//fork failed
			perror("fork");
			exit(1);
		
		case 0:			//child, 關閉讀的功能,只留寫
			dup2(p[1],STDOUT_FILENO);
			close(p[0]);
			execvp("ls",command);
			
		default:		//parent, 關閉寫的功能,只留讀
			close(p[1]);
			while(read(p[0], buf, 1024)!=0){
				printf("%s",buf);
			}
			break;
	}
	
	sleep(1);
	return 0;
} 
