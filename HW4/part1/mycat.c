#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h>

#define BUFSIZE 256

int main(int argc, char **argv){
	FILE *fd;
	char buf[BUFSIZE];
	size_t sread;
	
	if(argc!=2){
		return 1;
	}
	
	fd = fopen("123","r");
	if(fd == NULL){
		perror("open");
		exit(1);
	}
	
	while((fgets(buf, BUFSIZE, fd))!=0){
		printf("%s", buf);
	}
	
	return 0;
}
