#include <unistd.h>
#include "mocked_functions.h"

int __wrap_recv(int fd, char *buf, size_t len, int flags){
	return read(fd, buf, len);
}

