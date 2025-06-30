#include "../includes/http_server.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../includes/debug.h"
int main(){
	printf_dbg("debugging enabled\n");
	struct sockaddr_in a;
	a.sin_family = AF_INET;
	a.sin_addr.s_addr = inet_addr("0.0.0.0");
	a.sin_port = htons(80);
	run(a, 20, 0);
}
