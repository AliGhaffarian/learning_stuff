#ifndef TEST_SOURCE_SOCKET_UTILITIES
#define TEST_SOURCE_SOCKET_UTILITIES

//include cmocka
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <cmocka.h>

#include <sys/socket.h>
#include <fcntl.h>

#include "generic_test_context.h"

typedef struct {
	int resulted_errno;
	int returnval;
	char *read_string;
}result_recv_until_str;
typedef struct {
	char *read_from_buffer;
	char *str;
	result_recv_until_str expected;
}test_case_recv_until_str;
void test_recv_until_str(void **);
void display_context_recv_until_str(test_context *);
void tear_down_test_recv_until_str(test_context *);
bool test_source_socket_utilities();
#endif
