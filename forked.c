#include <signal.h>
#include <stdio.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

void sig_chld(int), sig_int(int);

int main(int argc, char *argv[]) {
	int srvSock, cliSock;

	if (argc < 2)
		exitError("ERROR: port number not provided\n",1);

	srvSock = tcpListen(argv[1]);
	signal(SIGCHLD, sig_chld);
	signal(SIGINT, sig_int);

	while (1) {
		cliSock = accept(srvSock, NULL, NULL);
		if (cliSock == -1)
			exitError("ERROR: could not accept incoming connection\n", 1);

		switch (fork()) {
			case -1:
				close(cliSock);
				close(srvSock);
				exitError("ERROR: could not fork child process",1);
				break;
			case 0:
				close(srvSock);
				handleHttpRequest(cliSock);
				closeWriteSock(cliSock);
				exit(EXIT_SUCCESS);
				break;
			default:
				close(cliSock);
				break;
		}
	}

	close(srvSock);
	return 0;
}

void sig_int(int signo) {
	void pr_cpu_time(void);
	pr_cpu_time();
	exit(0);
}

void sig_chld(int signo) {
	pid_t pid;
	int stat;

	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);

	return;
}

