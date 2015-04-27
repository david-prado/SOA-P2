#include <signal.h>
#include <stdio.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

void sig_int(int);

int main(int argc, char *argv[]) {
	int srvSock, cliSock;

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

	close(srvSock);
	return 0;
}

void sig_int(int signo) {
	void pr_cpu_time(void);
	pr_cpu_time();
	exit(0);
}
