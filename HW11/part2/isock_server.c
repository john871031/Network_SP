/*
 * isock_server : listen on an internet socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include "dict.h"

int main(int argc, char **argv) {
	static struct sockaddr_in server,client;
	struct hostent *host;
	struct utsname name;
	int sd,cd,n;
	Dictrec tryit;
	int clientsize = sizeof(struct sockaddr_in);
	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(1);
	}

	/* Create the socket.
	 * Fill in code. */
	if (uname(&name) == -1) {
		perror("uname");
		exit(1);
	}
	if ((host = gethostbyname(name.nodename)) == NULL) {
		perror("gethostbyname");
		exit(1);
	}
	memset((char *) &server, '\0', sizeof(server));	
	server.sin_family = AF_INET;
	//server.sin_addr.s_addr = htonl(INADDR_ANY);
    	server.sin_port = htons(PORT);
    	memcpy( (char *) &server.sin_addr,host->h_addr,host->h_length);
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
	    	perror("socket");
	    	exit(1);
	}
	/* Initialize address.
	 * Fill in code. */
	
	/* Name and activate the socket.
	 * Fill in code. */
	if(bind(sd, (struct sockaddr*)&server, sizeof(server)) == -1){
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
					/* Lookup the word , handling the different cases appropriately */
					read(cd, &tryit, sizeof(Dictrec));
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to the client. */
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
				exit(0); /* child does not want to be a parent */

			default :
				close(cd);
				break;
		} /* end fork switch */
	} /* end forever loop */
} /* end main */
