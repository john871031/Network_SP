#include "svmsg_file.h"
#include <unistd.h>
#include <errno.h>
static void grimReaper(int sig){
    int savedErrno;
    savedErrno = errno;                 /* waitpid() might change 'errno' */
    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;
    errno = savedErrno;
}
static void serveRequest(const struct requestMsg *req, int *seq){
    struct responseMsg resp;
    resp.mtype = req->clientId;
    resp.seqNum = *seq;
    if (msgsnd(req->clientId, &resp, REQ_MSG_SIZE, 0) == -1){
        errExit("msgsnd");
     }
    *seq += req->seqLen;
}
int main(int argc, char *argv[]){
    struct requestMsg req;
    pid_t pid;
    ssize_t msgLen;
    int serverId;
    struct sigaction sa;
    int seqNum = 0;
    int pipefd_O[2];
    int pipefd_T[2];
    close(pipefd_O[0]);
    close(pipefd_T[1]);
    if (pipe(pipefd_O) == -1){
    	perror("pipe");
    	exit(errno);
    }
    if (pipe(pipefd_T) == -1){
    	perror("pipe");
    	exit(errno);
    }
    serverId = msgget(SERVER_KEY, IPC_CREAT | S_IRUSR | S_IWUSR | S_IWGRP);
    if (serverId == -1)
        errExit("msgget");
    for (;;) {
        msgLen = msgrcv(serverId, &req, REQ_MSG_SIZE, 0, 0);
        if (msgLen == -1) {
            if (errno == EINTR)         /* Interrupted by SIGCHLD handler? */
                continue;               /* ... then restart msgrcv() */
            errMsg("msgrcv");           /* Some other error */
            break;                      /* ... so terminate loop */
        }
        pid = fork();                   /* Create child process */
        if (pid == -1) {
            errMsg("fork");
            break;
        }
        if (pid == 0) {                 /* Child handles request */
            close(pipefd_O[1]);
            close(pipefd_T[0]);
            read(pipefd_O[0], &seqNum, sizeof(seqNum));
            serveRequest(&req,&seqNum);
            write(pipefd_T[1], &seqNum, sizeof(seqNum));
            _exit(EXIT_SUCCESS);
        }else{
        	
        	write(pipefd_O[1], &seqNum ,sizeof(seqNum));
        	read(pipefd_T[0], &seqNum, sizeof(seqNum));
        }
    }
    if (msgctl(serverId, IPC_RMID, NULL) == -1)
        errExit("msgctl");
    exit(EXIT_SUCCESS);
}
