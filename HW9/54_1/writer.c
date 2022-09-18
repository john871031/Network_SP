#include "semun.h"              /* Definition of semun union */
#include "svshm_xfr.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

#define SHMNAME "/demo_shm"

//SEM_KEY    /* Key for semaphore set */
//SHM_KEY    /* Key for shared memory segment */
int main(int argc, char *argv[]){
    int semid, shmid, bytes, xfrs;
    struct shmseg *shmp;
    union semun dummy;
    size_t len;
    semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS);
    if (semid == -1)
        errExit("semget");

    if (initSemAvailable(semid, WRITE_SEM) == -1)    //WRITE_SEM = 0 
        errExit("initSemAvailable");
    if (initSemInUse(semid, READ_SEM) == -1)
        errExit("initSemInUse");

    shmid = shm_open(SHMNAME, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (shmid == -1)
        errExit("shm_open");
    
    len = strlen(argv[2]);
    if(ftruncate(shmid, sizeof(struct shmseg)) == -1)
        errExit("ftruncate");
 
    shmp = mmap(NULL, sizeof(struct shmseg), PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);
    if (shmp == MAP_FAILED)
        errExit("mmap");

    /* Transfer blocks of data from stdin to shared memory */

    for (xfrs = 0, bytes = 0; ; xfrs++, bytes += shmp->cnt) {
        if (reserveSem(semid, WRITE_SEM) == -1)         /* Wait for our turn */
            errExit("reserveSem");

        shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE);
        if (shmp->cnt == -1)
            errExit("read");

        if (releaseSem(semid, READ_SEM) == -1)          /* Give reader a turn */
            errExit("releaseSem");

        /* Have we reached EOF? We test this after giving the reader
           a turn so that it can see the 0 value in shmp->cnt. */

        if (shmp->cnt == 0)
            break;
    }

    /* Wait until reader has let us have one more turn. We then know
       reader has finished, and so we can delete the IPC objects. */

    if (reserveSem(semid, WRITE_SEM) == -1)
        errExit("reserveSem");

    if (semctl(semid, 0, IPC_RMID, dummy) == -1)
        errExit("semctl");
    if (close(shmid) == -1)
        errExit("close");
    

    fprintf(stderr, "Sent %d bytes (%d xfrs)\n", bytes, xfrs);
    exit(EXIT_SUCCESS);
}
