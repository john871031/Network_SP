#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
	int inputFd, openFlags, opt;
	int file = 0;
	mode_t filePerms;
	ssize_t numRead, numWrite, numWrite2;
	char buf[BUF_SIZE];

	if(argc>1)
		file = 1;

	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	while((opt = getopt(argc, argv, "a"))!=-1){
		switch(opt){
			case 'a':
				openFlags = O_CREAT | O_WRONLY | O_APPEND;
		}
	}

	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;		//rw-rw-rw
	inputFd = open(argv[argc-1], openFlags, filePerms);

	numRead = read(STDIN_FILENO, buf, BUF_SIZE);
	if(numRead == -1)
		perror("read");
	
	numWrite = write(STDOUT_FILENO, buf, numRead);
	if(numWrite == -1)
		perror("write");

	if(file){
		numWrite2 = write(inputFd,buf,numRead);
		if(numWrite2 == -1){
			perror("write");
		}
	}
	return 0;
}
