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

static struct params tParams;
struct timeval start;
struct timeval end;

int main(int argc, char *argv[]){
    int nThreads, n, *tnum;
    pthread_t *tid;

    if (argc < 6)
       exitError("Usage: client <hostname|IP> <port#> <file> <N-threads> <N-cicles>\n", 1);

    tParams.hostname = argv[1];
    tParams.port = argv[2];
    tParams.file = argv[3];
    nThreads = atoi(argv[4]);
    tParams.nCicles = atoi(argv[5]);
    tid = calloc(nThreads,sizeof(pthread_t));

    if(gettimeofday(&start, NULL)==-1)
    	exitError("ERROR: could not get time",1);

	for (n = 0; n < nThreads; n++) {
		tnum = malloc(sizeof(int));
		*tnum = n;
		if (pthread_create(&tid[n], NULL, &thread_start, (void *) tnum))
			exitError("ERROR: pthread_create", 1);
	}

	signal(SIGINT, sig_int);

	for (n=0;n<nThreads;n++)
		pthread_join(tid[n], NULL);

	if(gettimeofday(&end,NULL)==-1)
    	exitError("ERROR: could not get time",1);

	printStats();
	free(tid);
    return 0;
}

void * thread_start(void * args) {
	int sock,n, tnum;

	tnum = *((int *)args);
	free(args);
	printf("Client thread %d started\n",tnum);

    for(n=0;n<(tParams.nCicles);n++){
    	sock = tcpConnect(tParams.hostname,tParams.port);
    	httpGet(sock,tParams.file);
    	close(sock);
    }

    printf("Client thread %d terminated\n",tnum);

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
	elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec-start.tv_usec)/1000000.0;

	printf("### STATISTICS ###\n");
	printf("User CPU time = %g\n", user);
	printf("System CPU time = %g\n", sys);
	printf("Elapsed time = %g\n", elapsed);
}
