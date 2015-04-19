/*
 * http.h
 *
 *  Created on: Apr 17, 2015
 *      Author: davidp
 */

#ifndef HTTP_H_
#define HTTP_H_

int handleHttpRequest(int clientSock);

void httpReply(int clientSock, const char *uri);

int httpGet(int serverSock, char * resource);

void getDate();

#endif /* HTTP_H_ */
