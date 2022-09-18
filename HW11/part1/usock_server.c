/*
 * usock_server : listen on a Unix socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include "dict.h"

int main(int argc, char **argv) {
    struct sockaddr_un server;
    struct sockaddr_un client;
    int sd,cd,n;
    Dictrec tryit;
    int clientsize = sizeof(struct sockaddr_un);
    if (argc != 3) {
      fprintf(stderr,"Usage : %s <dictionary source>"
          "<Socket name>\n",argv[0]);
      exit(errno);
    }

    /* Setup socket.
     * Fill in code. */
    
    /* Initialize address.
     * Fill in code. */
	
    /* Name and activate the socket.
     * Fill in code. */
    if((sd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
    	perror("socket");
    	exit(1);
    } 
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, argv[2]);
    unlink(server.sun_path);
    if(bind(sd, (struct sockaddr*)&server, sizeof(struct sockaddr_un)) == -1){
	perror("bind");
	exit(1);
    } 
    if(listen(sd, 128) == -1){
    	perror("listen");
	exit(1);
    }
    /* main loop : accept connection; fork a child to have dialogue */
    for (;;) {
		/* Wait for a connection.
		 * Fill in code. */
		if((cd = accept(sd, (struct sockaddr*)&client, &clientsize)) == -1){
			perror("connect");
			exit(1);
		}

		/* Handle new client in a subprocess. */
		switch (fork()) {
			case -1 : 
				DIE("fork");
			case 0 :
				close (sd);	/* Rendezvous socket is for parent only. */
				/* Get next request.
				 * Fill in code. */
				while (1) {
					read(cd, &tryit, sizeof(Dictrec));
					/* Lookup request. */
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to client. */
						case FOUND: 
							/* Fill in code. */
							write(cd, &tryit, sizeof(Dictrec));
							break;
						case NOTFOUND: 
							/* Fill in code. */
							strcpy(tryit.text, "XXXX");
							write(cd, &tryit, sizeof(Dictrec));
							break;
						case UNAVAIL:
							DIE(argv[1]);
					} /* end lookup switch */
					
				} /* end of client dialog */

				/* Terminate child process.  It is done. */
				exit(0);

			/* Parent continues here. */
			default :
				close(cd);
				break;
		} /* end fork switch */
    } /* end forever loop */
} /* end main */
