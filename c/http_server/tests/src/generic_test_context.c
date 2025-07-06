#include "generic_test_context.h"
#include <stdlib.h>
#include <errno.h>
//include cmocka
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <cmocka.h>

int generic_test_teardown(void **states){
	test_context *context = (test_context *)*states;
	if(context->success == false){
		print_message("----------------\n");
		context->display_context(context);
		print_message("----------------\n");
	}
	if (context->tear_down_func)
		context->tear_down_func(context);
	free(((test_context *)*states)->test_result);

	errno = 0;
	return 0;
}
