#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
	int inputFd,outputFd,openFlags;
	mode_t filePerms;
	ssize_t numRead;
	char buf[BUF_SIZE];

	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	//rw-rw-rw
	
	inputFd = open(argv[1], O_RDONLY);
	if(inputFd == -1)
		perror("file");

	outputFd = open(argv[2],openFlags,filePerms);
	if(outputFd == -1)
		perror("file");

	int i,holesize;
	while((numRead = read(inputFd,buf,BUF_SIZE))!=0){
		for(i=0;i<numRead;i++){
			if(buf[i] == '\0'){
				holesize++;
			}else if(holesize>0){
				lseek(outputFd, holesize, SEEK_CUR);
				write(outputFd,&buf[i],1);
				holesize = 0;
			}else
				write(outputFd,&buf[i],1);
		}
	}

	return 0;
}
