#include <signal.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

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
}

void sigInt(int signo) {
	int i;

	for (i = 0; i < nchildren; i++)
		kill(pids[i], SIGTERM);
	while (wait(NULL) > 0)
		;
	if (errno != ECHILD)
		exitError("ERROR: could not wait for child",1);

	exit(0);
}
