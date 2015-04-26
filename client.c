#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

void sig_int(int signo);
void * thread_start(void * args);

struct params{
	char *hostname;
	char *port;
	char *file;
	int nCicles;
};

int main(int argc, char *argv[]){
    int nThreads, n;
    pthread_t *tid;
    struct params tParams;

    if (argc < 6)
       exitError("Usage: client <hostname|IP> <port#> <file> <N-threads> <N-cicles>\n", 1);

    tParams.hostname = argv[1];
    tParams.port = argv[2];
    tParams.file = argv[3];
    nThreads = atoi(argv[4]);
    tParams.nCicles = atoi(argv[5]);
    tid = calloc(nThreads,sizeof(pthread_t));

	for (n = 0; n < nThreads; n++) {
		if (pthread_create(&tid[n], NULL, &thread_start, (void *) &tParams))
			exitError("ERROR: pthread_create", 1);
	}

	signal(SIGINT, sig_int);

	for (n=0;n<nThreads;n++)
		pthread_join(tid[n], NULL);

	free(tid);
    return 0;
}

void * thread_start(void * args) {
	int sock,n;
	struct params *tParams;

	tParams = ((struct params *)args);

    for(n=0;n<(tParams->nCicles);n++){
    	sock = tcpConnect(tParams->hostname,tParams->port);
    	httpGet(sock,tParams->file);
    	close(sock);
    }

	pthread_exit(0);
}

void sig_int(int signo) {
	int i;
	//void pr_cpu_time(void);

	//pr_cpu_time();

	//for (i = 0; i < nthreads; i++)
		//printf("thread %d, %ld connections\n", i, tptr[i].thread_count);

	exit(0);
}


