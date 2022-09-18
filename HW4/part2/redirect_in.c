/*
 * redirect_in.c  :  check for <
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "shell.h"
#define STD_OUTPUT 1
#define STD_INPUT  0

/*
 * Look for "<" in myArgv, then redirect input to the file.
 * Returns 0 on success, sets errno and returns -1 on error.
 */
int redirect_in(char ** myArgv) {
  	int i = 0;
  	int fd;

  	/* search forward for <
  	 *
	 * Fill in code. */
	
	while(myArgv[i] != NULL){
		if(*myArgv[i] == '<'){
			break;
		}
		i++;
	}
	
  	if (myArgv[i]) {	/* found "<" in vector. */

    	/* 1) Open file.
     	 * 2) Redirect stdin to use file for input.
   		 * 3) Cleanup / close unneeded file descriptors.
   		 * 4) Remove the "<" and the filename from myArgv.
		 *
   		 * Fill in code. */

   		fd = open(myArgv[i+1],O_RDONLY);

   		if(fd == -1){
   			perror("open: ");
			return -1;	
		}
		close(STD_INPUT);
		if(dup(fd)!=STD_INPUT){
			fprintf(stderr,"dup failed to return 0!\n");
			exit(1);
		}
		close(fd);
		//free(myArgv[i]);
		//free(myArgv[i+1]);
		myArgv[i] = NULL;
		myArgv[i+1] = NULL;
  	}
  	return 0;
}
