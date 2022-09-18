#define _XOPEN_SOURCE 600 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <ftw.h> 
#include <unistd.h>
#include <limits.h>

#define BUF_LEN (10 * sizeof(struct inotify_event) + NAME_MAX + 1)

int inotifyFd;

static int dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb){
	int wd;
	wd = inotify_add_watch(inotifyFd, pathname, IN_ALL_EVENTS);		//添加監控 item 
	if(wd == -1){
		perror("inotify_add_watch");
		exit(1);
	}
	return 0;
}
static void displayInotifyEvent(struct inotify_event *i){
	printf("	wd = %2d; ", i->wd);
	if(i->cookie > 0){
		printf("cookie = %4d; ", i->cookie);
	}
	
	printf("mask = ");
	if(i->mask & IN_ACCESS)		printf("IN_ACCESS ");
	if(i->mask & IN_ATTRIB)		printf("IN_ATTRIB ");
	if(i->mask & IN_CLOSE_NOWRITE)	printf("IN_CLOSE_NOWRITE ");
	if(i->mask & IN_CLOSE_WRITE)		printf("IN_CLOSE_WRITE ");
	if(i->mask & IN_CREATE)		printf("IN_CREATE ");
	if(i->mask & IN_DELETE)		printf("IN_DELETE ");
	if(i->mask & IN_DELETE_SELF)		printf("IN_DELETE_SELF ");
	if(i->mask & IN_IGNORED)		printf("IN_IGNORED ");
	if(i->mask & IN_ISDIR)			printf("IN_ISDIR ");
	if(i->mask & IN_MODIFY)		printf("IN_MODIFY ");
	if(i->mask & IN_MOVE_SELF)		printf("IN_MOVE_SELF ");
	if(i->mask & IN_MOVED_FROM)		printf("IN_MOVED_FROM ");
	if(i->mask & IN_MOVED_TO)		printf("IN_MOVED_TO ");
	if(i->mask & IN_OPEN)			printf("IN_OPEN ");
	if(i->mask & IN_Q_OVERFLOW)		printf("IN_Q_OVERFLOW ");
	if(i->mask & IN_UNMOUNT)		printf("IN_UNMOUNT ");
	printf("\n");
	if(i->len > 0){
		printf("	name = %s\n", i->name);
	}
}

int main(int argc, char *argv[]){
	int wd;		//watch descriptor
	int i;
	ssize_t numRead;
	char buf[BUF_LEN];
	char *p;
	struct inotify_event *event;
	
	if(argc < 2){
		printf("%s pathname...\n", argv[0]);
	}
	
	inotifyFd = inotify_init();		//create inotify , return file descriptor
	if(inotifyFd == -1){
		perror("inotify_init");
		exit(1);
	}
	if(nftw(argv[1], dirTree, 256, FTW_PHYS) == -1){	//通過指定目錄,搜尋其下所有目錄與檔案
		perror("nftw");
		exit(1);
	}
	printf("directory: %s\n", argv[1]);
	while(1){
		numRead = read(inotifyFd, buf, BUF_LEN);
		if(numRead == -1){
			perror("read: ");
			exit(1);
		}
		
		for(p = buf; p < buf + numRead; ){
			event = (struct inotify_event *)p;
			displayInotifyEvent(event);
			p += sizeof(struct inotify_event) + event->len; 
		} 
	}
	
	return 0;
} 
