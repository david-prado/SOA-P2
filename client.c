#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

void printStats();
void sig_int(int signo);
void * thread_start(void * args);

struct params{
	char *hostname;
	char *port;
	char *file;
	int nCicles;
};

struct timespec start;
struct timespec end;

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

    if(clock_gettime(CLOCK_REALTIME, &start)==-1)
    	exitError("ERROR: could not get time",1);

	for (n = 0; n < nThreads; n++) {
		if (pthread_create(&tid[n], NULL, &thread_start, (void *) &tParams))
			exitError("ERROR: pthread_create", 1);
	}

	signal(SIGINT, sig_int);

	for (n=0;n<nThreads;n++)
		pthread_join(tid[n], NULL);

	if(clock_gettime(CLOCK_REALTIME, &end)==-1)
    	exitError("ERROR: could not get time",1);

	printStats();
	free(tid);
    return 0;
}

void * thread_start(void * args) {
	int sock,n;
	struct params *tParams;

	printf("New client thread started\n");

	tParams = ((struct params *)args);

    for(n=0;n<(tParams->nCicles);n++){
    	sock = tcpConnect(tParams->hostname,tParams->port);
    	httpGet(sock,tParams->file);
    	close(sock);
    }

	pthread_exit(0);
}

void sig_int(int signo) {
	printStats();
	exit(0);
}

void printStats(){
	double user, sys, elapsed;
	struct rusage myusage;


	if (getrusage(RUSAGE_SELF, &myusage) < 0)
		exitError("ERROR: could not get resource usage",1);

	user = (double) myusage.ru_utime.tv_sec + myusage.ru_utime.tv_usec/1000000.0;
	sys = (double) myusage.ru_stime.tv_sec + myusage.ru_stime.tv_usec/1000000.0;

	elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec-start.tv_nsec)/1000000000.0;

	printf("### STATISTICS ###\n");
	printf("User CPU time = %g\n", user);
	printf("System CPU time = %g\n", sys);
	printf("Elapsed time = %g\n", elapsed);
}


