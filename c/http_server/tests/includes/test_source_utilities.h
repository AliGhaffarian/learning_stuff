#ifndef TEST_SOURCE_UTILITIES
#define TEST_SOURCE_UTILITIES
#include <stdbool.h>
#include <stddef.h>

typedef struct {
	char *buffer;
	char ch;
	int expected;
}test_case_skip_all_consecutive_char;
extern test_case_skip_all_consecutive_char test_case_list_skip_all_consecutive_char[];
void test_skip_all_consecutive_char(void **);

typedef struct {
	char *buffer;
	int expected;
}test_case_skip_until_trailing_whitespace_or_cr;
extern test_case_skip_until_trailing_whitespace_or_cr test_case_list_skip_until_trailing_whitespace_or_cr[];
void test_skip_until_trailing_whitespace_or_cr(void **);

bool test_source_utilities();
#endif
