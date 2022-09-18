#include <sys/types.h>
#include <time.h>
#include <stdio.h>

#define BUFSIZE 256

int main(){
	time_t t;
	struct tm *info;
	char buf[BUFSIZE];
	
	time(&t);
	info = localtime(&t);
	
	strftime(buf, BUFSIZE, "%b %d(%a), %Y %I:%M %p\n",info);
	printf("%s", buf);
	
	return 0;
}
