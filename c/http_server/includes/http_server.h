#ifndef HTTP_SERVER
#define HTTP_SERVER
#include <netinet/in.h>
#include "http.h"
void run(struct sockaddr_in, int, int);
void handle_client(int);
void handle_get(int, http_request);
void handle_head(int, http_request);
void handle_badrequest(int, http_request);
void handle_notimplemented(int);
void handle_server_err(int);
bool expect_string_when_recving(int, char *, int);

int recv_and_parse_http_request(int, http_request *);
char *make_unsupported_msg(char *);
char *make_header_content_length(int);
extern void (*http_handlers[])(int, http_request);
extern int busy_threads;

#endif
