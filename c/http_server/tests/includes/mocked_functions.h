#ifndef MOCKED_FUNCTIONS
#define MOCKED_FUNCTIONS

#include <stddef.h>

/*
 * calls read, used to pass pipes instead of sockets to functions for testing
 */
int __wrap_recv(int , char *, size_t , int);

#endif
