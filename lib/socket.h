#ifndef SOCKETS_H_
#define SOCKETS_H_

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

int tcpListen(char* port);
void closeWriteSock(int sock);
int tcpConnect(char* hostname, char* port);

#endif /* SOCKETS_H_ */
