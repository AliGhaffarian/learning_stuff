#ifndef HTTP 
#define HTTP

#include <stdbool.h>
#include <stdio.h>
#define CRLF "\r\n"
#define END_OF_HTTP_MESSAGE "\r\n\r\n"
#define HTTP_VERSION_PREFIX "HTTP/"
enum HTTP_METHOD {
	HTTP_GET,
	HTTP_HEAD,
	NUMBER_OF_HTTP_METHOD_ELEMENTS
};
extern const char *http_method2str[];

enum HTTP_HEADER_NAME_ENUM {
	NULL_HEADER, //used for empty headers or end of header list
	UNSPEC_HEADER, //to indicate the header was not supported

	//entity headers
	CONTENT_LENGTH,

	//request headers 
	
	//response headers 
	
	//general headers
	
	NUMBER_OF_HTTP_HEADER_NAME_ENUM_ELEMENTS	
};

typedef struct {
	enum HTTP_HEADER_NAME_ENUM header_name; // to be resolved via http_header_enum2str
	char *field_value;
}http_header;
size_t sizeof_http_header_array(http_header *);

extern const char *http_header_enum2str[];
enum HTTP_HEADER_NAME_ENUM http_header_str2enum(char *);

enum HTTP_MESSAGE_TYPE {
	HTTP_REQUEST,
	HTTP_RESPONSE,
	NUMBER_OF_HTTP_MESSAGE_TYPE_ELEMENTS
};
typedef struct {
	enum HTTP_MESSAGE_TYPE type;
	http_header *headers;
	char *http_version;
	bool readonly;
	char rest_of_message[];
}http_message;
int make_http_message(http_message *);

typedef struct {
	enum HTTP_MESSAGE_TYPE type;
	http_header *headers; //terminated by nulptr
	char *http_version;
	bool readonly;

	enum HTTP_METHOD method;
	char *uri;

	char *body;
}http_request;
int make_http_request(http_request *);

enum BODY_TYPE {
	FILE_PTR,
	BUFFER,
	NONE,
	NUMBER_OF_BODY_TYPE_ELEMENTS
};

enum HTTP_STATUS_CODE {
	STATUS_200_SUCCESS,
	STATUS_500_SERVERERR,
	STATUS_404_NOTFOUND,
	STATUS_400_BADREQUEST,
	NUMBER_OF_HTTP_STATUS_CODE_ELEMENTS
};
typedef struct{
	enum HTTP_MESSAGE_TYPE type;
	http_header *headers;
	char *http_version;
	bool readonly;

	enum HTTP_STATUS_CODE status_code;

	// if body is fd, sendfile() will be used to transmit the data
	enum BODY_TYPE body_type;
	union {
		FILE *file;
		char *buffer;
	}body;
}http_response;
int make_http_response(http_response *);
extern http_response http_500_servererr;
extern http_response http_404_notfound;
extern http_response http_400_badrequest;
void init_default_http_messages(char *, http_header*);

typedef struct{
	int code;
	char *phrase;
}status_code;

extern const status_code http_status_enum2struct[];

bool add_header_to_http_message(http_message *, enum HTTP_HEADER_NAME_ENUM, char *);

void free_http_message(http_message *);
void free_http_request(http_message *);
void free_http_response(http_message *);
extern void (*free_http_message_handlers[])(http_message *);


char *parse_http_request(char *buffer, http_message *message);
char *parse_request_line(char *buffer, http_message *message);
char *parse_headers(char *buffer, http_message *message);
char *parse_body(char *buffer, http_message *message);

char *parse_method(char *buffer, http_request *request);
char *parse_uri(char *buffer, http_request* request);
char *parse_http_version(char *buffer, http_request *request);
char *parse_next_http_header(char *buffer, http_message *message);

#define HTTP_REQUEST_PARSERS_LEN 3
extern char *(*http_request_parsers[])(char *, http_message *);

#define HTTP_REQUEST_LINE_PARSERS_LEN 3
extern char *(*http_request_line_parsers[])(char *, http_request *);

void display_http_request(http_request *);
void display_http_response(http_response *);
#endif
