#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

int main(int argc, char *argv[]) {
	int srvSock, cliSock;

	if (argc < 2)
		exitError("ERROR: port number not provided\n",1);

	srvSock = tcpListen(argv[1]);

	//cliLen = sizeof(cliAddr);
	while (1) {
		//cliSock = accept(srvSock, (struct sockaddr *) &cliAddr, &cliLen);
		cliSock = accept(srvSock, NULL, NULL);
		if (cliSock == -1)
			exitError("ERROR: could not accept incoming connection\n", 1);

		handleHttpRequest(cliSock);
		closeWriteSock(cliSock);
	}

	close(srvSock);
	return 0;
}
