#ifndef TEST_SOURCE_HTTP
#define TEST_SOURCE_HTTP
#include "generic_test_context.h"
#include "http.h"

typedef struct{
	http_header parsed_header;
	int returnval;
}result_parse_next_http_header;
typedef struct{
	char *buffer;
	result_parse_next_http_header expected;
}test_case_parse_next_http_header;
extern test_case_parse_next_http_header test_case_list_parse_next_http_header[];
void test_parse_next_http_header(void **);
void display_context_parse_next_http_header(test_context *);

typedef struct{
	enum HTTP_HEADER_NAME_ENUM returnval;
}result_http_header_str2enum;
typedef struct{
	char *buffer;
	result_http_header_str2enum expected;
}test_case_http_header_str2enum;
extern test_case_http_header_str2enum test_case_list_http_header_str2enum[];
void test_http_header_str2enum(void **);
void display_context_http_header_str2enum(test_context *);

bool test_source_http();
#endif
