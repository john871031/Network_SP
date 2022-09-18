#include <unistd.h>
#include <sys/utsname.h> 
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 1024

int main(){
	
	struct utsname uts;
	char buf[BUFSIZE]; 
	int num;
	
	if(uname(&uts)==-1){
		perror("hostinfo.c:main:uname");
		exit(1);
	}
	
	//num = sysinfo(SI_HW_SERIAL, buf, BUFSIZE);
	/*if(num==-1){
		perror("sysinfo");
		exit(1);
	}*/
	
	printf("hostname: %s\n", uts.nodename);
	printf("%s\n", uts.release);
	printf("hostid: %ld\n", gethostid());
	
	return 0;
}

