#include <signal.h>
#include <stdio.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

void sig_int(int);
void pr_cpu_time(void);

static int srvSock;

int main(int argc, char *argv[]) {
	int cliSock;

	if (argc < 2)
		exitError("ERROR: port number not provided\n",1);

	srvSock = tcpListen(argv[1]);

	signal(SIGINT, sig_int);
	while (1) {
		cliSock = accept(srvSock, NULL, NULL);
		if (cliSock == -1)
			exitError("ERROR: could not accept incoming connection\n", 1);

		handleHttpRequest(cliSock);
		closeWriteSock(cliSock);
	}
}

void sig_int(int signo) {
	pr_cpu_time();
	close(srvSock);
	exit(0);
}
