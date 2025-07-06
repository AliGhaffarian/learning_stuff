#include <stdlib.h>
#include <errno.h>
//include cmocka
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <cmocka.h>

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "generic_test_context.h"
#include "socket_utilities.h"
#include "test_source_socket_utilities.h"

test_case_recv_until_str test_case_list_recv_until_str[] = {
	{
		.read_from_buffer = "sometxtDELIMsometxt", 
		.str = "DELIM",
		.expected = {
			.resulted_errno = 0,
			.returnval = 12,
			.read_string = "sometxtDELIM"
		}
	},
	{
		.read_from_buffer = "GET / HTTP/1.1\r", 
		.str = "\r\n",
		.expected = {
			.resulted_errno = EBADMSG,
			.returnval = -1,
		}
	}
};

static test_context test_cases[] = {
	{
		.display_context = display_context_recv_until_str, 
		.test_case = (void *)&test_case_list_recv_until_str[0], 
		.success = false, 
		.tear_down_func = tear_down_test_recv_until_str 
	},

	{
		.display_context = display_context_recv_until_str, 
		.test_case = (void *)&test_case_list_recv_until_str[1], 
		.success = false, 
		.tear_down_func = tear_down_test_recv_until_str 
	}
};

const struct CMUnitTest source_socket_utilities_testers[] = {
	cmocka_unit_test_prestate_setup_teardown(test_recv_until_str, NULL, generic_test_teardown, &test_cases[0]),
	cmocka_unit_test_prestate_setup_teardown(test_recv_until_str, NULL, generic_test_teardown, &test_cases[1]),
};

void test_recv_until_str(void ** states){
	test_context *context = TEST_CONTEXT_PTR_FROM_STATES(states);
	test_case_recv_until_str *current_test_case = TEST_CASE_PTR_FROM_STATES(states);

	context->test_result = malloc(sizeof(result_recv_until_str));
	result_recv_until_str *test_result = context->test_result;
	test_result->read_string = malloc(strlen(current_test_case->read_from_buffer));

	int pipes[2];
	pipe(pipes);
	write(
			pipes[1],
			current_test_case->read_from_buffer, 
			strlen(current_test_case->read_from_buffer));
	close(pipes[1]);

	test_result->returnval = recv_until_str(
			pipes[0], 
			current_test_case->str, 
			test_result->read_string, 
			strlen(current_test_case->read_from_buffer), 
			0
			);
	test_result->resulted_errno = errno;

	assert_int_equal(test_result->returnval, current_test_case->expected.returnval);
	assert_int_equal(test_result->resulted_errno, current_test_case->expected.resulted_errno);
	if(current_test_case->expected.resulted_errno == 0)
		assert_string_equal(current_test_case->expected.read_string, test_result->read_string);
	context->success = true;	
}

void tear_down_test_recv_until_str(test_context *context){
	result_recv_until_str *test_result = (result_recv_until_str*)context->test_result;
	free(test_result->read_string);
}

void display_context_recv_until_str(test_context *context){
	test_case_recv_until_str *current_test_case = context->test_case;
	result_recv_until_str *expected = &current_test_case->expected;
	result_recv_until_str *test_result = context->test_result;

	print_message("expected results:\n");
	if(expected->resulted_errno == 0){
		print_message("returnval: %d\n", expected->returnval);
		print_message("buffer: %s\n", expected->read_string);
	}
	print_message("errno: %d\n", expected->resulted_errno);

	print_message("got results:\n");
	if(expected->resulted_errno == 0){
		print_message("returnval: %d\n", test_result->returnval);
		print_message("buffer: %s\n", test_result->read_string);
	}
	print_message("errno: %d\n", test_result->resulted_errno);
}

bool test_source_socket_utilities(){
	return cmocka_run_group_tests(source_socket_utilities_testers, NULL, NULL);
}
