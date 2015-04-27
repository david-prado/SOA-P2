#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void exitError(const char *msg, int err) {
	if(errno != 0)
		perror(msg);
	else
		fprintf(stderr,"%s",msg);
	exit(err);
}

void error(const char *msg){
	perror(msg);
}
