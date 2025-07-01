#include <stdbool.h>

//include cmocka
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <cmocka.h>

#include <string.h>
#include <test_source_utilities.h>
#include <utilities.h>

test_case_skip_all_consecutive_char test_case_list_skip_all_consecutive_char[] = {
	{.buffer = "haha haha", .ch = ' ', .expected = 0},
	{.buffer = "  haha haha", .ch = ' ', .expected = 2},
	{.buffer = "haha", .ch = ' ', .expected = 0},
};

const struct CMUnitTest utilities_source_testers[] ={
	cmocka_unit_test_prestate(test_skip_all_consecutive_char, &test_case_list_skip_all_consecutive_char[0]),	
	cmocka_unit_test_prestate(test_skip_all_consecutive_char, &test_case_list_skip_all_consecutive_char[1]),	
	cmocka_unit_test_prestate(test_skip_all_consecutive_char, &test_case_list_skip_all_consecutive_char[2]),	

	cmocka_unit_test_prestate(test_skip_until_trailing_whitespace_or_cr, &test_case_list_skip_until_trailing_whitespace_or_cr[0]),
	cmocka_unit_test_prestate(test_skip_until_trailing_whitespace_or_cr, &test_case_list_skip_until_trailing_whitespace_or_cr[1]),
	cmocka_unit_test_prestate(test_skip_until_trailing_whitespace_or_cr, &test_case_list_skip_until_trailing_whitespace_or_cr[2]),
};
void test_skip_all_consecutive_char(void **states){
	test_case_skip_all_consecutive_char *current_test_case = (test_case_skip_all_consecutive_char *)*states;

	int current_result = skip_all_consecutive_char(current_test_case->buffer, current_test_case->ch) - current_test_case->buffer;

	assert_int_equal(current_result, current_test_case->expected);
}

test_case_skip_until_trailing_whitespace_or_cr test_case_list_skip_until_trailing_whitespace_or_cr[] = {
	{.buffer = "haha ", .expected = 4},
	{.buffer = "a\r", .expected = 1},
	{.buffer = "aa", .expected = 2},
	{.buffer = "haha     ha  hahaha   \r", .expected = 19},
	{.buffer = "haha ha hahaha\r", .expected = 14}
};
void test_skip_until_trailing_whitespace_or_cr(void **states){
	test_case_skip_until_trailing_whitespace_or_cr *current_test_case = (test_case_skip_until_trailing_whitespace_or_cr *)*states;
	int current_result = skip_until_trailing_whitespace_or_cr(current_test_case->buffer) - current_test_case->buffer;
	assert_int_equal(current_result, current_test_case->expected);
}

bool test_source_utilities(){
	return cmocka_run_group_tests(utilities_source_testers, NULL, NULL);
}
