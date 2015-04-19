#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

void requestHandler(int cliSock){
	pid_t child;

	child = fork();
	if(child==-1)
		exitError("ERROR: could not fork process",1);

	if(child==0){
		handleHttpRequest(cliSock);
		closeWriteSock(cliSock);
		exit(EXIT_SUCCESS);
	}else{
		return;
	}
}

int main(int argc, char *argv[]) {
	int srvPort;

	if (argc < 2)
		exitError("ERROR: port number not provided\n",1);

	srvPort = atoi(argv[1]);
	signal(SIGCHLD, SIG_IGN);
	startServer(srvPort,&requestHandler);

	return 0;
}
