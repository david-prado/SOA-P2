#include "lib/http.h"
#include "lib/socket.h"
#include "lib/error.h"
#include <sys/select.h>
#include <signal.h>

void sig_int(int);
pid_t child_make(int, int);
void child_main();
void pr_cpu_time(void);

typedef struct {
	pid_t pid; 	/* process ID */
	int pipe; 	/* parent's stream pipe to/from child */
	int status; /* 0 = ready */
	long nConn; /* # connections handled */
} Child;

Child *child; 	/* array of Child structures; calloc'ed */

static int nChildren;

int main(int argc, char **argv) {
	int srvSock, i, nAvail, maxfd, nsel, cliSock, rc;
	ssize_t n;
	fd_set rset, masterset;

	if(argc < 3)
		exitError("Usage: pre-forkedv2 <port#> <#children>\n",1);

	srvSock = tcpListen(argv[1]);

	FD_ZERO(&masterset);
	FD_SET(srvSock, &masterset);
	maxfd = srvSock;

	nChildren = atoi(argv[2]);
	nAvail = nChildren;
	child = calloc(nChildren, sizeof(Child));

	/* 4prefork all the children */
	for (i = 0; i < nChildren; i++) {
		child_make(i, srvSock); /* parent returns */
		FD_SET(child[i].pipe, &masterset);
		maxfd = maxfd >= child[i].pipe ? maxfd:child[i].pipe;
	}

	signal(SIGINT, sig_int);

	for (;;) {
		rset = masterset;
		if (nAvail <= 0)
			FD_CLR(srvSock, &rset); /* turn off if no available children */
		nsel = select(maxfd + 1, &rset, NULL, NULL, NULL);

		/* 4check for new connections */
		if (FD_ISSET(srvSock, &rset)) {
			cliSock = accept(srvSock, NULL, NULL);

			for (i = 0; i < nChildren; i++)
				if (child[i].status == 0)
					break; /* available */

			if (i == nChildren)
				exitError("no available children",1);

			child[i].status = 1; /* mark child as busy */
			child[i].nConn++;
			nAvail--;

			n = Write_fd(child[i].pipe, "", 1, cliSock);
			close(cliSock);
			if (--nsel == 0)
				continue; /* all done with select() results */
		}

		/* find any newly-available children */
		for (i = 0; i < nChildren; i++) {
			if (FD_ISSET(child[i].pipe, &rset)) {
				if ((n = read(child[i].pipe, &rc, 1)) == 0)
					exitError("Child terminated unexpectedly", 1);
				child[i].status = 0;
				nAvail++;
				if (--nsel == 0)
					break; /* all done with select() results */
			}
		}
	}
}

void sig_int(int signo) {
	int i;

	/* terminate all children */
	for (i = 0; i < nChildren; i++)
		kill(child[i].pid, SIGTERM);
	while (wait(NULL) > 0)
		/* wait for all children */
		;
	if (errno != ECHILD)
		exitError("ERROR: could not wait for child process",1);

	pr_cpu_time();

	for (i = 0; i < nChildren; i++)
		printf("Worked child %d with pid %ld handled %ld connections\n", i, (long) child[i].pid, child[i].nConn);

	exit(0);
}

pid_t child_make(int i, int srvSock) {
	int sockfd[2];
	pid_t pid;

	socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);

	pid = fork();
	switch (pid) {
		case 0:/*child*/
			dup2(sockfd[1], STDERR_FILENO); /* child's stream pipe to parent */
			close(sockfd[0]);
			close(sockfd[1]);
			close(srvSock); /* child does not need this open */
			child_main(); /* never returns */
			break;
		case -1:
			exitError("ERROR: could not fork process", 1);
			break;
		default:/*parent*/
			printf("Forked worked process %d with pid %ld\n", i, (long) pid);
			close(sockfd[1]);
			child[i].pid = pid;
			child[i].pipe = sockfd[0];
			child[i].status = 0;
	}

	return(pid); /* parent */
}

void child_main() {
	char c;
	int cliSock;

	for (;;) {
		if (Read_fd(STDERR_FILENO, &c, 1, &cliSock) == 0)
			exitError("read_fd returned 0",1);
		if (cliSock < 0)
			exitError("no descriptor from read_fd",1);

		handleHttpRequest(cliSock); /* process request */
		closeWriteSock(cliSock);

		write(STDERR_FILENO, "", 1); /* tell parent we're ready again */
	}
}
