#ifndef GENERIC_TEST
#define GENERIC_TEST

#include <stdbool.h>
typedef struct test_context_struct{
	bool success;
	void(*display_context)(struct test_context_struct *);
	void *test_case;
	void *test_result;
}test_context;
#define TEST_CONTEXT_PTR_FROM_STATES(states) (((test_context *)*states)->test_case)
#define TEST_CASE_PTR_FROM_STATES(states) (((test_context *)*states)->test_case)

int generic_test_setup(void **);
int generic_test_teardown(void **);
#endif
