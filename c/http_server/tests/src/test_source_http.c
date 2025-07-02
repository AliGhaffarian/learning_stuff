//include cmocka
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <cmocka.h>


#include "test_source_http.h"
#include "http.h"

test_case_parse_next_http_header test_case_list_parse_next_http_header[] = {
	{.buffer = "Content-Length: 233\r\n", .expected_header = {.header_name = CONTENT_LENGTH, .field_value = "233"}, .expected_return_offset = 19},
	{.buffer = "other_Content-Length: 233\r\n", .expected_header = {.header_name = NULL_HEADER, .field_value = "233"}, .expected_return_offset = 25},
	{.buffer = "Content-Length:    233    \r\n", .expected_header = {.header_name = NULL_HEADER, .field_value = "233"}, .expected_return_offset = 22}
};

const struct CMUnitTest source_http_testers[] = {
	cmocka_unit_test_prestate(test_parse_next_http_header, &test_case_list_parse_next_http_header[0]),
	cmocka_unit_test_prestate(test_parse_next_http_header, &test_case_list_parse_next_http_header[1]),
	cmocka_unit_test_prestate(test_parse_next_http_header, &test_case_list_parse_next_http_header[2]),
};

void test_parse_next_http_header(void **states){
	test_case_parse_next_http_header *current_test_case = (test_case_parse_next_http_header *)*states;
	http_message tmp_message;
	make_http_message(&tmp_message);
	int current_return_result = parse_next_http_header(current_test_case->buffer, &tmp_message) - current_test_case->buffer;

	assert_string_equal(current_test_case->expected_header.field_value, tmp_message.headers[0].field_value);
	assert_int_equal(current_test_case->expected_header.header_name, tmp_message.headers[0].header_name);
	assert_int_equal(current_test_case->expected_return_offset, current_return_result);
}

bool test_source_http(){
	return cmocka_run_group_tests(source_http_testers, NULL, NULL);
}
