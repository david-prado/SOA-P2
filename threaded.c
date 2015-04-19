#include <pthread.h>
#include <stdlib.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

void * thread_start(void * args){
	int *cliSock = (int *) args;
	handleHttpRequest(*cliSock);
	closeWriteSock(*cliSock);
	return 0;
}

void requestHandler(int cliSock){
	pthread_attr_t attr;
	pthread_t thread_id;
	int s;

	s = pthread_attr_init(&attr);
	if (s != 0)
		exitError("ERROR: pthread_attr_init", 1);

	s = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	if (s != 0)
		exitError("ERROR: pthread_attr_setdetachstate", 1);

	s = pthread_create(&thread_id, &attr, &thread_start, &cliSock);
	if (s != 0)
		exitError("ERROR: pthread_create",1);

	s = pthread_attr_destroy(&attr);
	if (s != 0)
		exitError("ERROR: pthread_attr_destroy",1);

}

int main(int argc, char *argv[]) {
	int srvPort;

	if (argc < 2)
		exitError("ERROR: port number not provided\n",1);

	srvPort = atoi(argv[1]);
	startServer(srvPort,&requestHandler);

	return 0;
}
