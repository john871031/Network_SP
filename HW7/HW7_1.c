#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

int num;

static void handler(int sig){
}

void _sigsuspend(){
	struct sigaction sa;
	sigset_t blockedMask, emptyMask;
	pid_t pid;
	int i;
	clock_t start, end;
	double total;
	
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if(sigaction(SIGUSR1, &sa, NULL) == -1){
		printf("sigaction");
		exit(1);
	} 
	
	sigemptyset(&blockedMask);
	sigaddset(&blockedMask, SIGUSR1);
	if(sigprocmask(SIG_SETMASK, &blockedMask, NULL) == -1){
		printf("sigprocmask");
		exit(1);
	}
	sigemptyset(&emptyMask);
	
	switch(pid = fork()){
		case -1:
			printf("fork");
			exit(1);
		case 0:		//child
			start = clock();
			for(i = 0; i < num; i++){
				if(kill(getppid(), SIGUSR1) == -1){
					printf("kill");
					exit(1);
				}
				if(sigsuspend(&emptyMask) == -1 && errno != EINTR){		//EINTR interrupt system call
					printf("sigsuspend"); 
					exit(1);
				}
			}
			end = clock();
			printf("sigsuspend: %f\n", (double)(end - start) / CLOCKS_PER_SEC);
			exit(EXIT_SUCCESS);
		default:
			for(i = 0; i < num; i++){
				if(sigsuspend(&emptyMask) == -1 && errno != EINTR){
					printf("sigsuspend");
					exit(1);
				}
				if(kill(pid, SIGUSR1) == -1){
					printf("kill");
					exit(1);
				}
			}	
			sleep(1);
		}
}

void _sigwaitinfo(){
	struct sigaction sa;
	sigset_t blockedMask;
	siginfo_t info;
	pid_t pid;
	int i;
	clock_t start, end;
	double total;
	
	sigemptyset(&blockedMask);
	sigaddset(&blockedMask, SIGUSR1);
	if(sigprocmask(SIG_SETMASK, &blockedMask, NULL) == -1){
		printf("sigprocmask");
		exit(1);
	}
	
	switch(pid = fork()){
		case -1:
			printf("fork");
			exit(1);
		case 0:
			start = clock();
			for(i = 0; i < num; i++){
				if(kill(getppid(), SIGUSR1) == -1){
					printf("kill");
					exit(1);
				}
				if(sigwaitinfo(&blockedMask, &info) == -1 && errno != EINTR){		//EINTR interrupt system call
					printf("sigsuspend"); 
					exit(1);
				}
			}
			end = clock();
			printf("sigwaitinfo: %f\n", (double)(end - start) / CLOCKS_PER_SEC);
			exit(EXIT_SUCCESS);
		default:
			for(i = 0; i < num; i++){
				if(sigwaitinfo(&blockedMask, &info) == -1 && errno != EINTR){		//EINTR interrupt system call
					printf("sigwaitinfo"); 
					exit(1);
				}
				if(kill(pid, SIGUSR1) == -1){
					printf("kill");
					exit(1);
				}
			}
			sleep(1);
			exit(EXIT_SUCCESS);
	}
}

int main(int argc, char *argv[]){
	
	if(argc != 2 || strcmp(argv[1], "--help") == 0){
		printf("%s num-sigs\n", argv[0]);
		exit(1);
	}
	num = atoi(argv[1]);
	_sigsuspend();
	_sigwaitinfo();
	
	return 0;
} 
