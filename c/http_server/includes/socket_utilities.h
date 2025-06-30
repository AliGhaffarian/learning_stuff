#ifndef SOCK_UTILS
#define SOCK_UTILS
#include <sys/types.h>
#include <netinet/in.h>

size_t inet_sock_len(struct sockaddr);

#endif
