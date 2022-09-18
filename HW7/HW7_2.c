
#define _XOPEN_SOURCE 500

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
//#include <sys/wait.h>
//#include <sys/types.h>

//void _help(){
//	printf("1: _sigset(), 2: _sighold(), 3: _sigrelse, 4: _sigignore(), 5: _sigpause()\n");
//	exit(1);
//} 
typedef void (*sighandler_t)(int);

static void handler(int sig){
	printf("\nin handler\n");
}

int _sighold(int sig){		//新增 
	sigset_t blockMask;
	sigemptyset(&blockMask);
	sigaddset(&blockMask, sig);
	
	return sigprocmask(SIG_BLOCK, &blockMask, NULL);
}
static sighandler_t _sigset(int sig, sighandler_t handler){
	sigset_t setMask;
	struct sigaction sa, oldSa;
	if(sigprocmask(SIG_BLOCK, NULL, &setMask) == -1){
		return (sighandler_t)-1;
	}
	sa.sa_handler = handler;
	if(sigaction(sig, &sa, &oldSa) == -1){
		return (sighandler_t)-1;
	}
	if(handler == SIG_HOLD){
		_sighold(sig); 
		return oldSa.sa_handler;
	}
	if(sigismember(&setMask, sig)){
		return SIG_HOLD;
	}else{
		return oldSa.sa_handler;
	}
		
}

int _sigrelse(int sig){		//刪除 
	sigset_t unblockMask;
	sigemptyset(&unblockMask);
	sigaddset(&unblockMask, sig);
	
	return sigprocmask(SIG_UNBLOCK, &unblockMask, NULL);
}
int _sigignore(int sig){
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	
	return sigaction(sig, &sa, NULL);
}
int _sigpause(int sig){		//從mask 中刪除一個指定的signal
	sigset_t pMask;
	if(sigprocmask(SIG_BLOCK, NULL, &pMask) == -1){		 
		printf("sigprocmask error");
		exit(1);
	}
	sigdelset(&pMask, sig);
	
	return sigsuspend(&pMask);
}

int main(){
	int option;
	struct sigaction sa;
	sigset_t blockMask;
	
	sa.sa_handler = handler;		
	sa.sa_flags = 0;
	/*while(1){		//test sigset()
		if(_sigset(SIGINT, handler) == (sighandler_t)-1){
			printf("_sigset error\n");
			exit(1);
		}
		sleep(1);
		printf("use CTRL C to see handler\n");
	}*/
	
	/*while(1){		//test sighold()
		if(_sighold(SIGINT) == -1){
			printf("_sighold error\n");
			exit(1);
		}
		sleep(1);
		printf("SIGINT is blocked, CTRL C will not stop\n");
	}*/
	
	/*for(int i = 0; i < 5; i++){		//test sigrelse()
		if(_sighold(SIGINT) == -1){
			printf("_sighold error\n");
			exit(1);
		}
		sleep(1);
		printf("SIGINT is blocked, CTRL C will not stop\n");
	}
	while(1){
		if(_sigrelse(SIGINT) == -1){
			printf("_sigrelse error\n");
			exit(1);
		}
		sleep(1);
		printf("SIGINT is deleted, CTRL C can stop\n");
	}*/
	
	/*while(1){		//test sigignore()
		if(_sigignore(SIGINT) == -1){
			printf("_sigignore error\n");
			exit(1);
		}
		sleep(1);
		printf("SIGINT is ignored, CTRL C will not stop\n");
	}*/
	
	/*if(sigaction(SIGINT, &sa, NULL) == -1){	//test sigpause()
		printf("_sigpause error\n");		//阻擋進入handler, 直到輸入CTRL C, 進入handler, 並返回
		exit(1);
	}
	printf("goto sigsuspend, until input CTRL C\n");
	if(_sigpause(SIGINT) == 0){
		printf("_sigpause error\n");
		exit(1);
	}
	printf("back\n");*/
	
	return 0;
}
