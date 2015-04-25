#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "error.h"
#include "socket.h"

int tcpListen(int srvPort) {
	int srvSock, cliSock;
	socklen_t cliLen;
	struct sockaddr_in srvAddr, cliAddr;

	if (srvPort < 0)
		exitError("ERROR: port number not provided\n", 1);

	//Create a socket
	srvSock = socket(AF_INET, SOCK_STREAM, 0);
	if (srvSock < 0)
		exitError("ERROR: could not open socket\n", 1);

	// Setup the sockaddr_in before bind()
	bzero((char *) &srvAddr, sizeof(srvAddr));
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.s_addr = INADDR_ANY;
	srvAddr.sin_port = htons(srvPort);

	//Bind the socket to a "well-known" address
	if (bind(srvSock, (struct sockaddr *) &srvAddr, sizeof(srvAddr)) < 0)
		exitError("ERROR: could not bind socket to specified address\n", 1);

	listen(srvSock, 10);

	return srvSock;
}

void closeWriteSock(int sock){
	int res;
	char buffer[4000];

	shutdown(sock, SHUT_WR);

	for (;;) {
		res = read(sock, buffer, 4000);
		if (res < 0)
			exitError("ERROR: could not read from socket",1);
		if (!res)
			break;
	}

	close(sock);
}
