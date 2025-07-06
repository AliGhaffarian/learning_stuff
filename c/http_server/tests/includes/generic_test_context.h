#ifndef GENERIC_TEST
#define GENERIC_TEST

#include <stdbool.h>
typedef struct test_context_struct{
	bool success;
	void(*display_context)(struct test_context_struct *);
	void (*tear_down_func)(struct test_context_struct*); //to free allocated stuff in test_case, test_result
	void *test_case;
	void *test_result;
}test_context;
#define TEST_CONTEXT_PTR_FROM_STATES(states) ((test_context *)*states)
#define TEST_CASE_PTR_FROM_STATES(states) (((test_context *)*states)->test_case)

int generic_test_setup(void **);
int generic_test_teardown(void **);
#endif
