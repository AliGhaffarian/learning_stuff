#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//include cmocka
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <cmocka.h>


#include "test_source_http.h"
#include "http.h"
#include "generic_test_context.h"

test_case_parse_next_http_header test_case_list_parse_next_http_header[] = {
	{
		.buffer = "Content-Length: 233\r\n", 
		.expected = {
			.parsed_header = {.header_name = CONTENT_LENGTH, .field_value = "233"}, 
			.returnval = 19
		}
	},

	{
		.buffer = "other_Content-Length: 233\r\n", 
		.expected = {
			.parsed_header = {.header_name = UNSPEC_HEADER, .field_value = "233"}, 
			.returnval = 25
		}
	},
	{
		.buffer = "Content-Length:    233    \r\n", 
		.expected = {
			.parsed_header = {.header_name = CONTENT_LENGTH, .field_value = "233"}, 
			.returnval = 26
		}
	},
	{
		.buffer = "Content-Length:    233    \n", 
		.expected = {
			.returnval = -1 // error value, since in a valid state the lowerbound of returnval would be 0 (offset == buffer)
		}
	},
	{
		.buffer = "Content-Length:    233    \r", 
		.expected = {
			.returnval = -1 // error value, since in a valid state the lowerbound of returnval would be 0 (offset == buffer)
		}
	}
};

test_case_http_header_str2enum test_case_list_http_header_str2enum[] = {
	{
		.buffer = "Content-Length", 
		.expected = {.returnval = CONTENT_LENGTH}
	},
	{
		.buffer = "Content_Length", 
		.expected = {.returnval = UNSPEC_HEADER}
	},
	{
		.buffer = "", 
		.expected = {.returnval = NULL_HEADER}
	},
	{
		.buffer = "a", 
		.expected = {.returnval = UNSPEC_HEADER}
	},
	{
		.buffer = "aaaa", 
		.expected = {.returnval = UNSPEC_HEADER}
	},
};

test_context test_cases[] = {
	{.display_context = display_context_parse_next_http_header, .test_case = (void *)&test_case_list_parse_next_http_header[0], .success = false},
	{.display_context = display_context_parse_next_http_header, .test_case = (void *)&test_case_list_parse_next_http_header[1], .success = false},
	{.display_context = display_context_parse_next_http_header, .test_case = (void *)&test_case_list_parse_next_http_header[2], .success = false},
	{.display_context = display_context_parse_next_http_header, .test_case = (void *)&test_case_list_parse_next_http_header[3], .success = false},
	{.display_context = display_context_parse_next_http_header, .test_case = (void *)&test_case_list_parse_next_http_header[4], .success = false},

	{.display_context = display_context_http_header_str2enum, .test_case = (void *)&test_case_list_http_header_str2enum[0], .success = false},
	{.display_context = display_context_http_header_str2enum, .test_case = (void *)&test_case_list_http_header_str2enum[1], .success = false},
	{.display_context = display_context_http_header_str2enum, .test_case = (void *)&test_case_list_http_header_str2enum[2], .success = false},
	{.display_context = display_context_http_header_str2enum, .test_case = (void *)&test_case_list_http_header_str2enum[3], .success = false},
	{.display_context = display_context_http_header_str2enum, .test_case = (void *)&test_case_list_http_header_str2enum[4], .success = false},
};

const struct CMUnitTest source_http_testers[] = {
	cmocka_unit_test_prestate_setup_teardown(test_parse_next_http_header, NULL, generic_test_teardown, &test_cases[0]),
	cmocka_unit_test_prestate_setup_teardown(test_parse_next_http_header, NULL, generic_test_teardown, &test_cases[1]),
	cmocka_unit_test_prestate_setup_teardown(test_parse_next_http_header, NULL, generic_test_teardown, &test_cases[2]),
	cmocka_unit_test_prestate_setup_teardown(test_parse_next_http_header, NULL, generic_test_teardown, &test_cases[3]),
	cmocka_unit_test_prestate_setup_teardown(test_parse_next_http_header, NULL, generic_test_teardown, &test_cases[4]),

	cmocka_unit_test_prestate_setup_teardown(test_http_header_str2enum, NULL, generic_test_teardown, &test_cases[5]),
	cmocka_unit_test_prestate_setup_teardown(test_http_header_str2enum, NULL, generic_test_teardown, &test_cases[6]),
	cmocka_unit_test_prestate_setup_teardown(test_http_header_str2enum, NULL, generic_test_teardown, &test_cases[7]),
	cmocka_unit_test_prestate_setup_teardown(test_http_header_str2enum, NULL, generic_test_teardown, &test_cases[8]),
	cmocka_unit_test_prestate_setup_teardown(test_http_header_str2enum, NULL, generic_test_teardown, &test_cases[9]),
};

void test_parse_next_http_header(void **states){
	test_case_parse_next_http_header *current_test_case = (test_case_parse_next_http_header *)TEST_CASE_PTR_FROM_STATES(states);
	http_message tmp_message;
	make_http_message(&tmp_message);
	char *copy_of_buffer = strdup(current_test_case->buffer);
	long current_returnval = parse_next_http_header(copy_of_buffer, &tmp_message) - copy_of_buffer;
	free(copy_of_buffer);

	test_context *context = (test_context *)*states;
	context->test_result = malloc(sizeof(result_parse_next_http_header));
	result_parse_next_http_header *ptr_to_context_test_result = (result_parse_next_http_header *)context->test_result;
	ptr_to_context_test_result->parsed_header = tmp_message.headers[0];
	ptr_to_context_test_result->returnval = current_returnval;


	// if header can't be parsed, the ptr won't be readable
	if(current_test_case->expected.returnval < 0)
		assert_true((long)current_returnval == -(long)copy_of_buffer);
	
	else{
		assert_int_equal(current_test_case->expected.returnval, current_returnval);
		assert_int_equal(current_test_case->expected.parsed_header.header_name, tmp_message.headers[0].header_name);
		assert_string_equal(current_test_case->expected.parsed_header.field_value, tmp_message.headers[0].field_value);
	}


	context->success = true;
}

void test_http_header_str2enum(void **states){
	test_case_http_header_str2enum *current_test_case = (test_case_http_header_str2enum *)TEST_CASE_PTR_FROM_STATES(states);
	enum HTTP_HEADER_NAME_ENUM current_returnval = http_header_str2enum(current_test_case->buffer);

	test_context *context = (test_context *)*states;
	context->test_result = malloc(sizeof(result_http_header_str2enum));
	result_http_header_str2enum *ptr_to_context_test_result = (result_http_header_str2enum *)context->test_result;
	ptr_to_context_test_result->returnval = current_returnval;

	assert_int_equal(current_test_case->expected.returnval, current_returnval);

	context->success = true;
}

int generic_test_teardown(void **states){
	test_context *context = (test_context *)*states;
	if(context->success == false){
		print_message("----------------\n");
		context->display_context(context);
		print_message("----------------\n");
	}
	free(((test_context *)*states)->test_result);
	return 0;
}

void display_context_http_header_str2enum(test_context *context){
	result_http_header_str2enum *expected = &((test_case_http_header_str2enum *)context->test_case)->expected;
	result_http_header_str2enum *test_result = (result_http_header_str2enum *)(context->test_result);
	print_message("expected: %s, got: %s\n", http_header_enum2str[expected->returnval], http_header_enum2str[test_result->returnval]);
}

void display_context_parse_next_http_header(test_context *context){
	result_parse_next_http_header *expected = &((test_case_parse_next_http_header *)context->test_case)->expected;
	result_parse_next_http_header *test_result = (result_parse_next_http_header *)context->test_result;

	print_message("expected results:\n");
	if (expected->returnval < 0){
		print_message("returnval: some ptr\n");
	}
	else{
		print_message("header: %s: %s\n", http_header_enum2str[expected->parsed_header.header_name], expected->parsed_header.field_value);
		print_message("returnval: %ld\n", expected->returnval);
	}

	print_message("got results:\n");
	if (expected->returnval < 0) {	
		print_message("returnval: %p\n", (void *)test_result->returnval);
	}
	else {
		print_message("header: %s: %s\n", http_header_enum2str[test_result->parsed_header.header_name], test_result->parsed_header.field_value);
		print_message("returnval: %ld\n", test_result->returnval);	
	}
}
bool test_source_http(){
	return cmocka_run_group_tests(source_http_testers, NULL, NULL);
}
