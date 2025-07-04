#ifndef SOCK_UTILS
#define SOCK_UTILS
#include <stddef.h>
#include <sys/types.h>
#include <netinet/in.h>

size_t inet_sock_len(struct sockaddr);
int recv_until_str(int, char *, char *, size_t, int);
#endif
