#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "svmsg_file.h"

static int clientId;

int main(int argc, char *argv[]){
    struct requestMsg req;
    struct responseMsg resp;
    int serverId, numMsgs;
    ssize_t mqLen, totBytes;
    mqd_t mqd_server, mqd_client;
    struct mq_attr attr;
    unsigned int prio;
	
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    if (strlen(argv[1]) > sizeof(req.pathname) - 1)
        cmdLineErr("pathname too long (max: %ld bytes)\n",
                (long) sizeof(req.pathname) - 1);
    /*FIXME: above: should use %zu here, and remove (long) cast */

    /* Get server's queue identifier; create queue for response */

    mqd_client = mq_open(mqclient, O_RDONLY | O_CREAT, 0666, NULL);
    if(mqd_client == -1){
	errExit("mq_open");
    }

    mqd_server = mq_open(mqserver, O_RDWR);
    if (mqd_server == -1){
    	errExit("mq_open");	
    }

    req.mtype = 1;                      /* Any type will do */
    req.clientId = getpid();
    strncpy(req.pathname, argv[1], sizeof(req.pathname) - 1);
    req.pathname[sizeof(req.pathname) - 1] = '\0';
                                        /* Ensure string is terminated */
    
    if (mq_send(mqd_server, (char *)&req, sizeof(struct requestMsg), 0) == -1)
        errExit("mq_send");

    /* Get first response, which may be failure notification */
    
    mqLen = mq_receive(mqd_client, (char *)&resp, RESP_MSG_SIZE, &prio);
    if (mqLen == -1)
        errExit("mq_receive");
    
    if (resp.mtype == RESP_MT_FAILURE) {
        printf("%s\n", resp.data);      /* Display msg from server */
        exit(EXIT_FAILURE);
    }

    /* File was opened successfully by server; process messages
       (including the one already received) containing file data */
    
    totBytes = mqLen;                  /* Count first message */
    for (numMsgs = 1; resp.mtype == RESP_MT_DATA; numMsgs++) {
        mqLen = mq_receive(mqd_client, (char *)&resp, RESP_MSG_SIZE, &prio);
        if (mqLen == -1)
            errExit("mq_receive");
            
        totBytes += mqLen;
    }

    printf("Received %ld bytes (%d messages)\n", (long) totBytes, numMsgs);
    /*FIXME: above: should use %zd here, and remove (long) cast (or perhaps
       better, make totBytes size_t, and use %zu)*/

    exit(EXIT_SUCCESS);
}
