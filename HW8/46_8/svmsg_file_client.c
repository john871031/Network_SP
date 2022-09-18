#include "svmsg_file.h"
static int clientId;

static void
removeQueue(void)
{
    if (msgctl(clientId, IPC_RMID, NULL) == -1)
        errExit("msgctl");
}

int
main(int argc, char *argv[])
{
    struct requestMsg req;
    struct responseMsg resp;
    int serverId, numMsgs;
    ssize_t msgLen, totBytes;
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);
    serverId = msgget(SERVER_KEY, S_IWUSR);
    if (serverId == -1)
        errExit("msgget - server message queue");
    clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP);
    if (clientId == -1)
        errExit("msgget - client message queue");
    if (atexit(removeQueue) != 0)
        errExit("atexit");
    req.mtype = 1;                    
    req.clientId = clientId;
    req.seqLen = (argc > 1) ? atoi(argv[1]) : 1;          
    if (msgsnd(serverId, &req, REQ_MSG_SIZE, 0) == -1)
        errExit("msgsnd");
    msgLen = msgrcv(clientId, &resp, REQ_MSG_SIZE, 0, 0);
    if (msgLen == -1)
        errExit("msgrcv");
    printf("%d\n", resp.seqNum);
    exit(EXIT_SUCCESS);
}
