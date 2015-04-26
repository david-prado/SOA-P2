#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

void sig_int(int signo);
void * thread_start(void * args);

int srvSock, nthreads;
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
	int i;
	pthread_t thread_id;

	if (argc < 3)
		exitError("Usage: pre-threaded <port#> <thread#>\n", 1);

	srvSock = tcpListen(argv[1]);
	nthreads = atoi(argv[2]);

	for (i = 0; i < nthreads; i++) {
		if (pthread_create(&thread_id, NULL, &thread_start, (void *) NULL))
			exitError("ERROR: pthread_create", 1);
	}

	signal(SIGINT, sig_int);

	for (;;)
		pause(); /* everything done by threads */
}

void * thread_start(void * args) {
	int cliSock;

	pthread_detach(pthread_self());

	while (1) {
		pthread_mutex_lock(&mlock);
		cliSock = accept(srvSock, NULL, NULL);
		pthread_mutex_unlock(&mlock);

		if (cliSock == -1)
			exitError("ERROR: could not accept incoming connection\n", 1);

		handleHttpRequest(cliSock);
		closeWriteSock(cliSock);
	}

	return NULL;
}

void sig_int(int signo) {
	int i;
	//void pr_cpu_time(void);

	//pr_cpu_time();

	//for (i = 0; i < nthreads; i++)
		//printf("thread %d, %ld connections\n", i, tptr[i].thread_count);

	exit(0);
}
