#ifndef HTTP_SERVER
#define HTTP_SERVER
#include <netinet/in.h>
#include <stddef.h>
#include "http.h"
#define SERVER_HTTP_VERSION "HTTP/1.1"
void run(struct sockaddr_in, int, int);
void handle_client(int);

//consumes the request
http_response process_get(http_request);
http_response process_head(http_request);

void handle_badrequest(int, http_request);
void handle_notimplemented(int);
void handle_server_err(int);
bool expect_string_when_recving(int, char *, int);

int recv_and_parse_http_request(int, http_request *);
char *make_unsupported_msg(char *);
char *make_header_content_length(int);
extern http_response (*http_processors[])(http_request);
extern int busy_threads;
extern size_t MAX_REQUEST_SIZE;
#endif
