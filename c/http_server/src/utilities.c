#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <utilities.h>
#include <debug.h>
void if_err_log_n_quit(bool did_err_occur){
	if (did_err_occur){
		printf("%s", strerror(errno));
		exit(errno);
	}
}

size_t file_size(FILE *file){
	int fd = fileno(file);
	long previous_position = ftell(file);
	lseek(fd, 0L, SEEK_END);
	long file_size = ftell(file);
	lseek(fd, previous_position, SEEK_SET);
	return file_size;
}

char *file_size_str(FILE *file){
	size_t size_int = file_size(file);
	char *buffer = malloc(sizeof(size_t)); // one for null byte at the end
	sprintf(buffer, "%ld", size_int); 
	return buffer;
}

void swap(char *first, char *second){
	*first ^= *second;
	*second ^= *first;
	*first ^= *second;
}

char *skip_all_consecutive_char(char *buffer, char ch){
	while (*buffer == ch)
		buffer++;
	return buffer;
}

char *skip_until_trailing_whitespace_or_cr(char *buffer){
	char *last_result = buffer;
	while(*buffer != '\r' && *buffer != 0){
		if(*buffer == ' '){
			last_result = buffer;
			buffer = skip_all_consecutive_char(buffer, ' ');
		}
		else {
			buffer++;
			last_result = buffer;
		}
	}
	return last_result;
}
