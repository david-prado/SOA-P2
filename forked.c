#include <signal.h>
#include <stdio.h>
#include "lib/error.h"
#include "lib/http.h"
#include "lib/socket.h"

void sig_chld(int);
void sig_int(int);
void pr_cpu_time(void);

static int srvSock;

int main(int argc, char *argv[]) {
	int cliSock;
	pid_t pid;

	if (argc < 2)
		exitError("ERROR: port number not provided\n",1);

	srvSock = tcpListen(argv[1]);
	signal(SIGCHLD, sig_chld);
	signal(SIGINT, sig_int);

	while (1) {
		cliSock = accept(srvSock, NULL, NULL);
		if (cliSock == -1)
			exitError("ERROR: could not accept incoming connection\n", 1);

		pid=fork();
		switch (pid) {
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
				printf("Forked new worker process with pid %d\n",pid);
				close(cliSock);
				break;
		}
	}
}

void sig_int(int signo) {
	pr_cpu_time();
	exit(0);
}

void sig_chld(int signo) {
	pid_t pid;
	int stat;

	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("Worker process with pid %d terminated with status %d\n", pid, stat);

	return;
}
