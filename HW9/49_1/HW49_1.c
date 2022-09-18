#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
	int fd1, fd2;
	struct stat statbuf;
	caddr_t inputaddr, outputaddr;
	
	if(argc != 3){
		printf("格式錯誤: ./cp test1.txt test2.txt\n");
		exit(1);
	}
	
	fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1){
		perror("open(fd1)");
		exit(1);
	}
	//fstat 要用文件描述符
	if(fstat(fd1, &statbuf) == -1){
		perror("fstat");
		exit(1);
	}
	
	inputaddr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd1, 0);
	if(inputaddr == MAP_FAILED){
		perror("mmap(inputaddr)");
		exit(1);
	}
	if(close(fd1) == -1){
		perror("close(fd1)");
		exit(1);
	}
	
	fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd2 == -1){
		perror("open(fd2)");
		exit(1);
	}
	if(ftruncate(fd2, statbuf.st_size) == -1){
		perror("ftruncate");
		exit(1);
	}
	outputaddr = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
	if(outputaddr == MAP_FAILED){
		perror("mmap(outputaddr)");
		exit(1);
	}
	memcpy(outputaddr, inputaddr, statbuf.st_size);
	if(close(fd2) == -1){
		perror("close(fd2)");
		exit(1);
	}
	return 0;
}
