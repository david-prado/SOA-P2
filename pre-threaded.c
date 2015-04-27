#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

#define	MAX_CLI	32

void pr_cpu_time(void);
void sig_int(int);
void * thread_start(void *);

typedef struct {
  pthread_t		tid;		/* thread ID */
  long			nConn;		/* # connections handled */
} Thread;

int					cliSocks[MAX_CLI], cliGet, cliPut;
pthread_mutex_t		cliMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t		cliCond = PTHREAD_COND_INITIALIZER;
static int nThreads;
Thread	*threads;		/* array of Thread structures; calloc'ed */


int main(int argc, char *argv[]) {
	int i, srvSock, cliSock, *tNum;

	if (argc < 3)
		exitError("Usage: pre-threaded <port#> <thread#>\n", 1);

	srvSock = tcpListen(argv[1]);
	nThreads = atoi(argv[2]);
	threads = calloc(nThreads,sizeof(Thread));

	for (i = 0; i < nThreads; i++) {
		tNum = malloc(sizeof(int));
		*tNum = i;
		if (pthread_create(&threads[0].tid, NULL, &thread_start, (void *) tNum))
			exitError("ERROR: pthread_create", 1);
	}

	signal(SIGINT, sig_int);

	cliGet = cliPut = 0;
	while(1){
		cliSock = accept(srvSock, NULL,NULL);
		if(cliSock==-1)
			exitError("ERROR: could not accept new connection on socket",1);
		pthread_mutex_lock(&cliMutex);
		cliSocks[cliPut] = cliSock;
		if (++cliPut == MAX_CLI)
			cliPut = 0;
		if (cliPut == cliGet)
			exitError("ERROR: cliPut = cliGet = %d", cliPut);
		pthread_cond_signal(&cliCond);
		pthread_mutex_unlock(&cliMutex);
	}
}

void * thread_start(void * args) {
	int cliSock;
	int tNum = *((int*) args);
	free(args);

	pthread_detach(pthread_self());
	printf("Pre-created worker thread %d\n", tNum);

	while(1){
    	pthread_mutex_lock(&cliMutex);
		while (cliGet == cliPut)
			pthread_cond_wait(&cliCond, &cliMutex);
		cliSock = cliSocks[cliGet];	/* connected socket to service */
		if (++cliGet == MAX_CLI)
			cliGet = 0;
		pthread_mutex_unlock(&cliMutex);
		threads[tNum].nConn++;
		handleHttpRequest(cliSock);
		closeWriteSock(cliSock);
	}

	return 0;
}

/* end serv08 */

void sig_int(int signo){
	int	i;

	pr_cpu_time();

	for (i = 0; i < nThreads; i++)
		printf("Worker thread %d, handled %ld connections\n", i, threads[i].nConn);

	exit(0);
}
