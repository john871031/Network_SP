/*
 * lookup8 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet TCP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_in server;
	struct hostent *host;
	struct utsname name;
	static int first_time = 1;

	if (first_time) {        /* connect to socket ; resource is server name */
		first_time = 0;

		/* Set up destination address. */
		if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
			perror("socket");
			exit(1);
		}
		
		/* Fill in code. */
		/* Allocate socket.
		 * Fill in code. */
		memset((char *) &server, '\0', sizeof(server));
		server.sin_family = AF_INET;
		//server.sin_addr.s_addr = htonl(INADDR_ANY);
		server.sin_port = htons(PORT);
		if (uname(&name) == -1) {
			perror("uname");
			exit(1);
		}
		if ((host = gethostbyname(name.nodename)) == NULL) {
			perror("gethostbyname");
			exit(1);
		}
		memcpy((char *)&server.sin_addr, host->h_addr,host->h_length);
		
		/* Connect to the server.
		 * Fill in code. */
		if(connect(sockfd, (struct sockaddr*)&server, sizeof(server)) == -1){
		 	perror("connect");
		 	exit(1);
		}
	}

	/* write query on socket ; await reply
	 * Fill in code. */
	write(sockfd, sought, sizeof(Dictrec));
	read(sockfd, sought, sizeof(Dictrec));
	if (strcmp(sought->text,"XXXX") != 0) {
		return FOUND;
	}

	return NOTFOUND;
}
