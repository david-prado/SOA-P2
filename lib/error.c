#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void exitError(const char *msg, int err) {
	perror(msg);
	exit(err);
}

void error(const char *msg){
	perror(msg);
}
