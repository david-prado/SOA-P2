#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "http.h"
#include "read_line.h"
#include "error.h"

int handleHttpRequest(int cliSock){
	char buffer[2048];
	char *request,*uri,*version;
	int n;

	//Read request
	n = readLine(cliSock, buffer, sizeof(buffer));
	if (n < 0)
		exitError("ERROR: could not read from socket",1);
	else if (n==0)
		error("ERROR: invalid HTTP GET request");

	request = strtok(buffer," ");
	uri = strtok(NULL," ");
		if(uri!=NULL && strlen(uri)>1)
			uri++;
	version = strtok(NULL," ");

	if(strcmp(request,"GET")==0)
		httpReply(cliSock,uri);
	else
		error("ERROR: invalid HTTP GET request");

	return 0;
}

void httpReply(int cliSock, const char *uri){
	char header[8190];
	struct stat sb;
	int fd;
	char date[200];
	getDate(date,sizeof(date));

	if(uri!=NULL && stat(uri,&sb)==0 && (sb.st_mode & S_IFMT) == S_IFREG){
		fd = open(uri,O_RDONLY);
		sprintf(header,""
				"HTTP/1.1 200 OK\n"
				"Date: %s\n"
				"Server: SOA/1.0 (Linux)\n"
				"Last-Modified: Sat, 18 Apr 2015 01:25:28 CST\n"
				"Content-Length: %d\n"
				"Content-Type: application/octet-stream\n"
				"Content-Disposition: attachment\n"
				"Connection: close\n\n",date,(int)sb.st_size);

		write(cliSock,header,strlen(header));
		sendfile(cliSock,fd,NULL,sb.st_size);
		close(fd);
	}
	else{
		sprintf(header,""
				"HTTP/1.1 404 NOT FOUND\n"
				"Date: %s\n"
				"Server: SOA/1.0 (Linux)\n"
				"Last-Modified: Sat, 18 Apr 2015 01:25:28 CST\n"
				"Content-Length: 0\n"
				"Content-Type: text/plain\n"
				"Connection: close\n\n404 Not Found",date);
		write(cliSock,header,strlen(header));
	}
}

void httpGet(int srvSock, char * file){
	ssize_t wr, wrTot;
	char request[2048];
	snprintf(request, sizeof(request), "GET /%s HTTP/1.1\n", file);

	printf("Request = %s",request);

	wrTot = 0;
	while(wrTot < strlen(request)){
		wr = write(srvSock+wrTot, request, strlen(request)-wrTot);
		if(wr==-1)
			exitError("ERROR: could not write to socket",1);
		wrTot += wr;
	}

	handleHttpResponse(srvSock);
}

void handleHttpResponse(int srvSock){
	char buffer[2048];
	char *version,*strCode;
	int intCode, n;

	//Read response
	n = readLine(srvSock, buffer, sizeof(buffer));
	if (n < 0)
		exitError("ERROR: could not read from socket",1);
	else if (n==0)
		exitError("ERROR: invalid HTTP GET response",1);

	printf("Status Line = %s",buffer);

	version = strtok(buffer," ");
	intCode = atoi(strtok(NULL," "));
	strCode = strtok(NULL," ");

	if(strcmp(strCode,"OK\n")!=0){
		printf("ERROR: HTTP Response Code = %d",intCode);
		exit(EXIT_FAILURE);
	}
	else{
		do{
			n = read(srvSock,buffer,sizeof(buffer));
			if(n==-1)
				exitError("ERROR: could not read from socket",1);
		}while(n!=0);
	}
}

void getDate(char *outstr, size_t len){
    time_t t;
    struct tm *tmp;

    t = time(NULL);
    tmp = localtime(&t);
    if (tmp == NULL)
        exitError("localtime",EXIT_FAILURE);

    if (strftime(outstr, len, "%a, %d %b %Y %T %Z", tmp) == 0)
        exitError("strftime returned 0",EXIT_FAILURE);
}
