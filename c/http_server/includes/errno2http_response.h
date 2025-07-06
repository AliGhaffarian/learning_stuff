#ifndef ERRNO2HTTP_RESPONSE
#define ERRNO2HTTP_RESPONSE
#include <stdbool.h>
#include "http.h"
//if errno is not mapped will do http_500_servererr
#define http_response_based_on_none_zero_errno(param_errno) (is_errno_registered(param_errno) ? *errno2http_response[param_errno] : http_500_servererr)
extern http_response *errno2http_response[];
extern size_t size_mapped_errnos;
extern int mapped_errnos[];
bool is_errno_registered(int);
#endif
