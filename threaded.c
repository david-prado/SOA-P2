#include <pthread.h>
#include <stdlib.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

void * thread_start(void * args){
	int cliSock = *((int *) args);
	free(args);
	pthread_detach(pthread_self());
	handleHttpRequest(cliSock);
	//closeWriteSock(cliSock);
	close(cliSock);
	return NULL;
}

int main(int argc, char *argv[]) {
	int srvSock, *cliSock;
	pthread_t thread_id;

	if (argc < 2)
		exitError("ERROR: port number not provided\n",1);

	srvSock = tcpListen(atoi(argv[1]));

	while (1) {
		cliSock = malloc(sizeof(int));
		*cliSock = accept(srvSock, NULL, NULL);
		if (*cliSock == -1)
			exitError("ERROR: could not accept incoming connection\n", 1);

		if (pthread_create(&thread_id, NULL, &thread_start, (void *) cliSock))
			exitError("ERROR: pthread_create",1);
	}

	close(srvSock);
	return 0;
}
