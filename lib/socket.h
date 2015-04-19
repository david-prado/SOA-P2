/*
 * sockets.h
 *
 *  Created on: Apr 18, 2015
 *      Author: davidp
 */

#ifndef SOCKETS_H_
#define SOCKETS_H_

void startServer(int port, void (*requestHandler)(int clientSock));
void closeWriteSock(int sock);

#endif /* SOCKETS_H_ */
