#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
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

test_case_parse_method test_case_list_parse_method[] = {
	{
		.buffer = "GET ",
		.expected = {
			.parsed_method = HTTP_GET,
			.returnval = 4,
			.resulted_errno = 0
		}
	},
	{
		.buffer = "HEAD ",
		.expected = {
			.parsed_method = HTTP_HEAD,
			.returnval = 5,
			.resulted_errno = 0
		}
	},
	{
		.buffer = "\nGET",
		.expected = {
			.parsed_method = HTTP_UNSPEC,
			.returnval = -1,
			.resulted_errno = EINVAL
		}
	},

};

static test_context test_cases[] = {
	{.display_context = display_context_parse_next_http_header, .test_case = (void *)&test_case_list_parse_next_http_header[0], .success = false, .tear_down_func = 0},
	{.display_context = display_context_parse_next_http_header, .test_case = (void *)&test_case_list_parse_next_http_header[1], .success = false, .tear_down_func = 0},
	{.display_context = display_context_parse_next_http_header, .test_case = (void *)&test_case_list_parse_next_http_header[2], .success = false, .tear_down_func = 0},
	{.display_context = display_context_parse_next_http_header, .test_case = (void *)&test_case_list_parse_next_http_header[3], .success = false, .tear_down_func = 0},
	{.display_context = display_context_parse_next_http_header, .test_case = (void *)&test_case_list_parse_next_http_header[4], .success = false, .tear_down_func = 0},

	{.display_context = display_context_http_header_str2enum, .test_case = (void *)&test_case_list_http_header_str2enum[0], .success = false, .tear_down_func = 0},
	{.display_context = display_context_http_header_str2enum, .test_case = (void *)&test_case_list_http_header_str2enum[1], .success = false, .tear_down_func = 0},
	{.display_context = display_context_http_header_str2enum, .test_case = (void *)&test_case_list_http_header_str2enum[2], .success = false, .tear_down_func = 0},
	{.display_context = display_context_http_header_str2enum, .test_case = (void *)&test_case_list_http_header_str2enum[3], .success = false, .tear_down_func = 0},
	{.display_context = display_context_http_header_str2enum, .test_case = (void *)&test_case_list_http_header_str2enum[4], .success = false, .tear_down_func = 0},

	{.display_context = display_context_parse_method, .test_case = (void *)&test_case_list_parse_method[0], .success = false, .tear_down_func = 0},
	{.display_context = display_context_parse_method, .test_case = (void *)&test_case_list_parse_method[1], .success = false, .tear_down_func = 0},
	{.display_context = display_context_parse_method, .test_case = (void *)&test_case_list_parse_method[2], .success = false, .tear_down_func = 0},
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


	cmocka_unit_test_prestate_setup_teardown(test_parse_method, NULL, generic_test_teardown, &test_cases[10]),
	cmocka_unit_test_prestate_setup_teardown(test_parse_method, NULL, generic_test_teardown, &test_cases[11]),
	cmocka_unit_test_prestate_setup_teardown(test_parse_method, NULL, generic_test_teardown, &test_cases[12]),
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

void test_parse_method(void ** states){
	test_case_parse_method *current_test_case = (test_case_parse_method *)TEST_CASE_PTR_FROM_STATES(states);
	http_request tmp_request;
	make_http_request(&tmp_request);

	test_context *context = TEST_CONTEXT_PTR_FROM_STATES(states);
	context->test_result = malloc(sizeof(result_parse_method));
	result_parse_method *ptr_to_context_test_result = (result_parse_method *)context->test_result;

	char *copy_of_buffer = strdup(current_test_case->buffer);
	long current_returnval = parse_method(copy_of_buffer, &tmp_request) - copy_of_buffer;
	free(copy_of_buffer);
	ptr_to_context_test_result->returnval = current_returnval;
	ptr_to_context_test_result->parsed_method = tmp_request.method;
	ptr_to_context_test_result->resulted_errno = errno;

	assert_int_equal(current_test_case->expected.resulted_errno, ptr_to_context_test_result->resulted_errno);
	if(current_test_case->expected.resulted_errno == 0){
		assert_int_equal(current_test_case->expected.parsed_method, ptr_to_context_test_result->parsed_method);
		assert_int_equal(current_test_case->expected.returnval, ptr_to_context_test_result->returnval);
	}
	context->success = true;
}

void display_context_parse_method(test_context *context){
	result_parse_method *expected = &((test_case_parse_method *)context->test_case)->expected;
	result_parse_method *test_result = (result_parse_method *)context->test_result;

	print_message("expected results:\n");
	if(expected->resulted_errno){
		print_message("errno: %d\n", expected->resulted_errno);
	}
	else{
		print_message("parsed method: %s\n", http_method2str[expected->parsed_method]);
		print_message("returnval: %ld\n", expected->returnval);
	}

	print_message("got:\n");
	if(expected->resulted_errno){
		print_message("errno: %d\n", test_result->resulted_errno);
	}
	else{
		print_message("parsed method: %s\n", http_method2str[test_result->parsed_method]);
		print_message("returnval: %ld\n", test_result->returnval);
	}
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
