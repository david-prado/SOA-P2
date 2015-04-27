#include <signal.h>
#include <stdio.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

void sig_int(int);

static int nchildren;
static pid_t *pids;

int main(int argc, char *argv[]) {
	int srvSock, cliSock, i;
	pid_t tempPid;

	if (argc < 3)
		exitError("Usage: pre-forked <port#> <#children>\n",1);

	srvSock = tcpListen(argv[1]);
	nchildren = atoi(argv[2]);
	pids = calloc(nchildren, sizeof(pid_t));

	for (i = 0; i < nchildren; i++) {
		tempPid = fork();
		switch (tempPid) {
			case -1:
				close(srvSock);
				exitError("ERROR: could not fork child process", 1);
				break;
			case 0:
				printf("Child %ld starting\n", (long) getpid());
				while (1) {
					cliSock = accept(srvSock, NULL, NULL);
					if (cliSock == -1)
						exitError("ERROR: could not accept incoming connection\n",1);

					handleHttpRequest(cliSock);
					closeWriteSock(cliSock);
				}
				close(srvSock);
				exit(EXIT_SUCCESS);
				break;
			default:
				pids[i] = tempPid;
				break;
		}
	}

	close(srvSock);

	signal(SIGINT, sig_int);

	for (;;)
		pause();
}

void
sig_int(int signo)
{
	int		i;
	void	pr_cpu_time(void);

		/* 4terminate all children */
	for (i = 0; i < nchildren; i++)
		kill(pids[i], SIGTERM);
	while (wait(NULL) > 0)		/* wait for all children */
		;
	if (errno != ECHILD)
		exitError("ERROR: could not wait for child",1);

	pr_cpu_time();
	exit(0);
}
