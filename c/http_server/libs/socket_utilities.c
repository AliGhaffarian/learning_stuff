#include <socket_utilities.h>
#include <netinet/in.h>
#include <sys/socket.h>

size_t inet_sock_len(struct sockaddr sock){
	switch (sock.sa_family) {
		case (AF_INET): return sizeof(struct sockaddr_in);
		case(AF_INET6): return sizeof(struct sockaddr_in6);	
	}
	return 0;
}
