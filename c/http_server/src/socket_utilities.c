#include "utilities.h"
#include <errno.h>
#include <netinet/in.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>

#include <socket_utilities.h>
size_t inet_sock_len(struct sockaddr sock){
	switch (sock.sa_family) {
		case (AF_INET): return sizeof(struct sockaddr_in);
		case(AF_INET6): return sizeof(struct sockaddr_in6);	
	}
	return 0;
}

int recv_until_str(int fd, char *str, char *buffer, size_t size_of_buffer, int flags){
	char *start_of_buffer = buffer;
	int total_read_bytes = 0;
	int iter_read_bytes = 0;
	char *str_offset = 0;

	long str_size = strlen(str);
	char *end_of_buffer = buffer + size_of_buffer;

	int last_iter_read_bytes = 0;
	do {
		buffer += iter_read_bytes;

		if(buffer >= end_of_buffer)
			goto bad_msg;	

		last_iter_read_bytes = iter_read_bytes;
		iter_read_bytes = recv(fd, buffer, size_of_buffer - total_read_bytes, flags);
		total_read_bytes += iter_read_bytes;

		if (iter_read_bytes == 0)
			goto bad_msg; // pass recv's errno
		if (iter_read_bytes == -1)
			return -1;
		if(iter_read_bytes < str_size){
			buffer -= iter_read_bytes; //to undo the addition in the beginning of loop
			continue;
		}

		//case of the str being sliced in half when recving
		char char_backup = 0;
		long end_of_slice = str_size <= iter_read_bytes ? str_size : iter_read_bytes;
		swap(buffer + end_of_slice, &char_backup);
		if (( str_offset = strstr(buffer - str_size + 1, str))){
			swap(buffer + str_size, &char_backup);
			break;
		}
		swap(buffer + end_of_slice, &char_backup);

		

	}while ( ! (str_offset = strstr(buffer, str)));

	str_offset[str_size] = 0;
	return str_offset + str_size - start_of_buffer;

bad_msg:
	errno = EBADMSG;
	return -1;
}
