#include <mqueue.h>
#include "svmsg_file.h"

static void             /* SIGCHLD handler */
grimReaper(int sig){
    int savedErrno;

    savedErrno = errno;                 /* waitpid() might change 'errno' */
    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;
    errno = savedErrno;
}

static void             /* Executed in child process: serve a single client */
serveRequest(const void *buf){
    int fd;
    ssize_t numRead;
    struct responseMsg resp;
    struct requestMsg *req;
    mqd_t mqd;
    struct mq_attr attr;
    
    mqd = mq_open(mqclient, O_RDWR);
    if(mqd == -1){
        perror("mq_open");
        exit(1);
    }
	
    req = (struct requestMsg *)buf;
	
    fd = open(req->pathname, O_RDONLY);
    if (fd == -1) {                     /* Open failed: send error text */
        resp.mtype = RESP_MT_FAILURE;
        snprintf(resp.data, sizeof(resp.data), "%s", "Couldn't open");
        mq_send(mqd, (char *)&resp, sizeof(resp.data) , 0);
        exit(EXIT_FAILURE);             /* and terminate */
    }

    /* Transmit file contents in messages with type RESP_MT_DATA. We don't
       diagnose read() and msgsnd() errors since we can't notify client. */

    resp.mtype = RESP_MT_DATA;
    while ((numRead = read(fd, resp.data, RESP_MSG_SIZE)) > 0)
        if (mq_send(mqd, (char *)&resp, numRead - 8, 0) == -1)
            break;

    /* Send a message of type RESP_MT_END to signify end-of-file */

    resp.mtype = RESP_MT_END;
    mq_send(mqd, (char *)&resp, 16, 0);         /* Zero-length mtext */
}

int main(int argc, char *argv[]){
    struct requestMsg req;
    pid_t pid;
    ssize_t mqLen;
    int serverId;
    struct sigaction sa;
    mqd_t mqd;
    struct mq_attr attr;
    void *buf;
    unsigned int prio;
	
    /* Create server message queue */
    mqd = mq_open(mqserver, O_RDONLY | O_CREAT, 0666, NULL);
    if(mqd == -1){
        perror("mq_open");
        exit(1);
    }
    if(mq_getattr(mqd, &attr) == -1){
        perror("mq_getattr");
        exit(1);
    }
    
    buf = malloc(attr.mq_msgsize);
    if(buf == NULL){
    	perror("malloc");
    	exit(1);
    }
    /* Establish SIGCHLD handler to reap terminated children */

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");

    /* Read requests, handle each in a separate child process */

    for (;;) {
        mqLen = mq_receive(mqd, buf, attr.mq_msgsize, &prio);
        if (mqLen == -1) {
            if (errno == EINTR)         /* Interrupted by SIGCHLD handler? */
                continue;               /* ... then restart msgrcv() */
            errMsg("mq_receive");           /* Some other error */
            break;                      /* ... so terminate loop */
        }

        pid = fork();                   /* Create child process */
        if (pid == -1) {
            errMsg("fork");
            break;
        }

        if (pid == 0) {                 /* Child handles request */
            serveRequest(buf);
            _exit(EXIT_SUCCESS);
        }

        /* Parent loops to receive next client request */
    }

    exit(EXIT_SUCCESS);
}
