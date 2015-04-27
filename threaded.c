#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

void sig_int(int);
void * thread_start(void *);

static int srvSock;

int main(int argc, char *argv[]) {
	int *cliSock;
	pthread_t thread_id;

	if (argc < 2)
		exitError("ERROR: port number not provided\n",1);

	srvSock = tcpListen(argv[1]);

	signal(SIGINT, sig_int);

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

void * thread_start(void * args){
	printf("Created new worker thread\n");
	int cliSock = *((int *) args);
	free(args);
	pthread_detach(pthread_self());
	handleHttpRequest(cliSock);
	closeWriteSock(cliSock);
	return NULL;
}

void sig_int(int signo){
    void    pr_cpu_time(void);

    pr_cpu_time();
    exit(0);
}
