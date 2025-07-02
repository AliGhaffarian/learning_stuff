#ifndef TEST_SOURCE_HTTP
#define TEST_SOURCE_HTTP
#include "http.h"

typedef struct{
	char *buffer;
	http_header expected_header;
	int expected_return_offset;
}test_case_parse_next_http_header;
extern test_case_parse_next_http_header test_case_list_parse_next_http_header[];
void test_parse_next_http_header(void **);

typedef struct{
	char *buffer;
	enum HTTP_HEADER_NAME_ENUM expected;
}test_case_http_method2str;
extern test_case_http_method2str test_case_list_http_method2str[];
void test_http_method2str(void **);

bool test_source_http();
#endif
