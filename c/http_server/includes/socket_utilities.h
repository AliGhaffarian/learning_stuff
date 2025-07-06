#ifndef SOCK_UTILS
#define SOCK_UTILS
#include <stddef.h>
#include <netinet/in.h>

size_t inet_sock_len(struct sockaddr);

/*
 * received from fd until str is found, places null byte after str, returns offset to the null byte
 * may read beyond the str
 */
int recv_until_str(int, char *, char *, size_t, int);
#endif
