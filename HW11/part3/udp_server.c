/*
 * udp_server : listen on a UDP socket ;reply immediately
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include "dict.h"

int main(int argc, char **argv) {
	static struct sockaddr_in server,client;
	int sockfd,siz;
	Dictrec dr, *tryit = &dr;
	int clientsize = sizeof(struct sockaddr_in);

	int len;
	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(errno);
	}

	/* Create a UDP socket.
	 * Fill in code. */
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
	    	perror("socket");
	    	exit(1);
	}
	/* Initialize address.
	 * Fill in code. */
	memset(&client, 0, sizeof(client));
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);
	/* Name and activate the socket.
	 * Fill in code. */
	if(bind(sockfd, (struct sockaddr*)&server, sizeof(server)) == -1){
		perror("bind");
		exit(1);
	}
	for (;;) { /* await client packet; respond immediately */
		
		siz = sizeof(client); /* siz must be non-zero */
		
		/* Wait for a request.
		 * Fill in code. */
		while (1) {
			/* Lookup request and respond to user. */
			len= recvfrom(sockfd, tryit, sizeof(dr), 0, (struct sockaddr*)&client, &siz);
			if(len == -1){
				perror("recvfrom");
				exit(1);
			}
			switch(lookup(tryit,argv[1]) ) {
				case FOUND: 
					/* Send response.
					 * Fill in code. */
					sendto(sockfd, tryit, sizeof(dr), 0, (struct sockaddr*)&client, siz);
					break;
				case NOTFOUND : 
					/* Send response.
					 * Fill in code. */
					strcpy(tryit->text, "XXXX");
					sendto(sockfd, tryit, sizeof(dr), 0, (struct sockaddr*)&client, siz);
					break;
				case UNAVAIL:
					DIE(argv[1]);
			} /* end lookup switch */
		} /* end while */
	} /* end forever loop */
} /* end main */
