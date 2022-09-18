#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define FIFO_TEMPLATE "/tmp/FIFOTEMPLATE"

int main(int argc, char *argv[]){
	int readFd, writeFd;                    
	int flags;
	
    	umask(0);	/* So we get the permissions we want */
    	
	if (mkfifo(FIFO_TEMPLATE, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST){
		printf("mkfifo %s", FIFO_TEMPLATE);
		exit(1);
    	}
    	//只打開write + O_NONBLOCK 會 fails 並返回(ENXIO)(6)
    	writeFd = open(FIFO_TEMPLATE, O_WRONLY | O_NONBLOCK);
	if (writeFd == -1){
		printf("errno on opening write end of FIFO: %d\n", errno);
	}
	errno = 0;
	
	//只打開 read + O_NONBLOCK 會 succeeds immediately
	readFd = open(FIFO_TEMPLATE, O_RDONLY | O_NONBLOCK);
	if (readFd == -1){
		printf("open %s", FIFO_TEMPLATE);
		exit(1);
	}
	
	//因read端有打開了, 所以write + O_NONBLOCK 不會fails並返回(ENXIO)(6)
	writeFd = open(FIFO_TEMPLATE, O_WRONLY | O_NONBLOCK);
	if (writeFd == -1){
		printf("errno on opening write end of FIFO: %d\n", errno);
		exit(1);
	}
	
	return 0;
}
