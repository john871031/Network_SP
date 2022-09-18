/*
 * lookup9 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet UDP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_in server, client;
	struct hostent *host;
	static int first_time = 1;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	
	if (first_time) {  /* Set up server address & create local UDP socket */
		first_time = 0;

		/* Set up destination address.
		 * Fill in code. */
		
		if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
			perror("socket");
			exit(1);
		}
		
		memset(&server, 0, sizeof(server));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = htonl(INADDR_ANY);
		server.sin_port = htons(PORT);
		/* Allocate a socket.
		 * Fill in code. */
		
	}

	/* Send a datagram & await reply
	 * Fill in code. */
	
	sendto(sockfd, sought, sizeof(Dictrec), 0, (struct sockaddr* )&server, addrlen);
	recvfrom(sockfd, sought, sizeof(Dictrec), 0, (struct sockaddr* )&server, &addrlen);
	
	if (strcmp(sought->text,"XXXX") != 0) {
		return FOUND;
	}

	return NOTFOUND;
}
