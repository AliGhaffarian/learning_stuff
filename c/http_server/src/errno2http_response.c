#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "errno2http_response.h"
#include "http.h"

http_response *errno2http_response[] = {
	[EPERM] = &http_500_servererr, //TODO: Add forbidden code
	[ENOENT] = &http_404_notfound,
	[EINVAL] = &http_400_badrequest,
	[EBADMSG] = &http_400_badrequest,
};

size_t size_mapped_errnos = 4;
int mapped_errnos[] = {
	EPERM,
	ENOENT,
	EINVAL,
	EBADMSG,
};

static int cmp_int(int *a, int *b){
	return (*a) - (*b);
}

bool is_errno_registered(int param_errno){
	int *found_ptr = (int *)bsearch(
			&param_errno, 
			mapped_errnos, 
			size_mapped_errnos, 
			sizeof(int), 
			(int(*)(const void*,const void*))cmp_int
			);

	return found_ptr; //if null pointer is returned, the returnval will be false automatically
}
