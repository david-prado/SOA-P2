#include <signal.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

int main(int argc, char *argv[]) {
	int srvSock, cliSock;

	if (argc < 2)
		exitError("ERROR: port number not provided\n",1);

	srvSock = tcpListen(argv[1]);
	signal(SIGCHLD, SIG_IGN);

	//cliLen = sizeof(cliAddr);
	while (1) {
		//cliSock = accept(srvSock, (struct sockaddr *) &cliAddr, &cliLen);
		cliSock = accept(srvSock, NULL, NULL);
		if (cliSock == -1)
			exitError("ERROR: could not accept incoming connection\n", 1);

		switch (fork()) {
			case -1:
				close(cliSock);
				close(srvSock);
				exitError("ERROR: could not fork child process",1);
				break;
			case 0:
				close(srvSock);
				handleHttpRequest(cliSock);
				closeWriteSock(cliSock);
				exit(EXIT_SUCCESS);
				break;
			default:
				close(cliSock);
				break;
		}
	}

	close(srvSock);
	return 0;
}
