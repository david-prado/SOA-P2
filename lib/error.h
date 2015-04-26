#ifndef ERROR_H_
#define ERROR_H_

#include <errno.h>

void exitError(const char *msg, int err);
void error(const char *msg);

#endif /* ERROR_H_ */
