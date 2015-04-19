#include <stdlib.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

void requestHandler(int cliSock){
	handleHttpRequest(cliSock);
	closeWriteSock(cliSock);
}

int main(int argc, char *argv[]) {
	int srvPort;

	if (argc < 2)
		exitError("ERROR: port number not provided\n",1);

	srvPort = atoi(argv[1]);
	startServer(srvPort,&requestHandler);

	return 0;
}
