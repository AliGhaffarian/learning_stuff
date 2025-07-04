#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <http.h>
#include <debug.h>
#include <utilities.h>

int make_http_message(http_message *message){
	message->headers = malloc(sizeof(http_header));
	if (message->headers == 0)
		return -1;

	message->headers->header_name = NULL_HEADER;
	printf_dbg("made a http header\n");
	return 0;
}
int make_http_request(http_request *request){
	request->type = HTTP_REQUEST;
	return make_http_message((http_message *)request);
}
int make_http_response(http_response *response){
	response->type = HTTP_RESPONSE;
	return make_http_message((http_message *)response);
}

const status_code http_status_enum2struct[] = {
	[STATUS_200_SUCCESS] = {.code = 200, .phrase = "OK"},
};

const char *http_header_enum2str[] = {
	[NULL_HEADER] = "",
	[UNSPEC_HEADER] = "not supported",
	[CONTENT_LENGTH] = "Content-Length",
	[NUMBER_OF_HTTP_HEADER_NAME_ENUM_ELEMENTS] = ""
};
enum HTTP_HEADER_NAME_ENUM http_header_str2enum(char *buffer){
	for(int i = 0; i < NUMBER_OF_HTTP_HEADER_NAME_ENUM_ELEMENTS; i++)
		if(!strcmp(buffer, http_header_enum2str[i]))
			return i;
	return UNSPEC_HEADER;
}

const char *http_method2str[NUMBER_OF_HTTP_METHOD_ELEMENTS] = {
	[HTTP_GET] = "GET",
	[HTTP_HEAD] = "HEAD",
	[HTTP_BAD_REQUEST] = "BAD REQUEST",
};

void (*free_http_message_handlers[])(http_message *message) = {
	[HTTP_REQUEST] = free_http_request,
	[HTTP_RESPONSE] = free_http_response
};
void free_http_header(http_header *header){
	if(header->field_value)
		free(header->field_value);
}
void free_http_message(http_message * message){
	if (message->http_version)
		free(message->http_version);
	http_header *current_header = message->headers;
	while(current_header->header_name != NULL_HEADER){
		free_http_header(current_header);
		current_header++;
	}
	if(message->headers)
		free(message->headers);

	return free_http_message_handlers[message->type](message);
}

void free_http_request(http_message *message){
	http_request *request = (http_request *)message;
	if (request->body)
		free(request->body);
}

void free_http_response(http_message *message){
	http_response *response = (http_response *)message;

	if (response->body_type == FILE_PTR){
		fclose(response->body.file);
	}
	else if (response->body_type == BUFFER &&
			response->body.buffer){
		free(response->body.buffer);
	}
}

size_t sizeof_http_header_array(http_header *headers){
	size_t size = 0;
	while(headers[size].header_name != NULL_HEADER)
		size++;
	size++; //NULL_HEADER
	return size;
}

bool add_header_to_http_message(http_message *message, enum HTTP_HEADER_NAME_ENUM header_name, char * field_value){
	size_t header_array_size = sizeof_http_header_array(message->headers);
	message->headers = realloc(
			message->headers, 
			( header_array_size + 1) * sizeof(http_header)); 
	if(message->headers == 0)
		return false;
	header_array_size++;

	message->headers[header_array_size-2].header_name = header_name;
	message->headers[header_array_size-2].field_value = strdup(field_value);

	message->headers[header_array_size-1].header_name = NULL_HEADER;
	printf_dbg(
			"added header %s, value %s\n", 
			http_header_enum2str[message->headers[header_array_size-2].header_name],
			message->headers[header_array_size-2].field_value
		  );
	return true;
}

/*
 * the order is significant because each one determine the starting offset of the next
 */
char *(*http_request_parsers[])(char *, http_message *) = {
	parse_request_line,
	parse_headers,
	parse_body
};
char *parse_http_request(char *buffer, http_message *message){
	http_request *request = (http_request *)message;

	for (int i = 0; i < HTTP_REQUEST_PARSERS_LEN; i++){
		buffer = http_request_parsers[i](buffer, message);
		if (buffer == 0){
			printf_dbg("%dth parsing failed\n", i);
			return 0;
		}

		if (!strncmp(buffer, END_OF_HTTP_MESSAGE, sizeof(END_OF_HTTP_MESSAGE) - 1))
			break; 
		else
			buffer += sizeof(CRLF) - 1;
	}

	//TODO fix parsers so this condition works normally
	if(false && strncmp(buffer, END_OF_HTTP_MESSAGE, sizeof(END_OF_HTTP_MESSAGE) - 1)){
		char buffer_char_backup = 0;
		swap(&buffer[sizeof(END_OF_HTTP_MESSAGE) - 1], &buffer_char_backup);
		printf_dbg("expected END_OF_HTTP_MESSAGE, got: '%s'\n", buffer);
		swap(&buffer[sizeof(END_OF_HTTP_MESSAGE) - 1], &buffer_char_backup);
		return 0;
	}
	
	buffer += sizeof(END_OF_HTTP_MESSAGE) - 1;
#ifdef DEBUG
	display_http_request(request);
#endif
	return buffer;
}

char *(*http_request_line_parsers[])(char *, http_request *) = {
	parse_method,
	parse_uri,
	parse_http_version
};
char *parse_request_line(char *buffer, http_message *message){
	http_request *request = (http_request *)message;

	for (int i = 0; i < HTTP_REQUEST_LINE_PARSERS_LEN; i++){
		buffer = http_request_line_parsers[i](buffer, request);
		if (buffer == 0){
			printf_dbg("%dth parsing failed\n", i);
			return 0;
		}
	}
	if(strncmp(buffer, CRLF, sizeof(CRLF) - 1))
		return 0;
	return buffer;
}

char *parse_method(char *buffer, http_request *request){
	int match = 1;
	int i;
	int current_method_len;
	for (i = 0; i < NUMBER_OF_HTTP_METHOD_ELEMENTS && match; i++){
		current_method_len = strlen(http_method2str[i]);
		match = strncmp(
				buffer, 
				http_method2str[i], 
				current_method_len
				);
		if(!match && buffer[current_method_len] != ' ')
			match = 1;
	}
	i--;
	if (match == 1)
		return 0;

	request->method = i; // how magical :D
			     
	printf_dbg("parsed http method: %s\n", http_method2str[request->method]);
	return buffer + current_method_len + 1; //start of next token
}
char *parse_uri(char *buffer, http_request *request){

	char *end_of_uri = strchr(buffer, ' ');
	if (end_of_uri == 0)
		return 0;

	if(*buffer == '/')
		buffer++;

	size_t uri_length = end_of_uri - buffer;
	if(uri_length == 0)
		return 0;

	request->uri = malloc(uri_length + 1);
	if (request->uri == 0)
		return 0;


	memcpy(request->uri, buffer, uri_length);
	request->uri[uri_length] = 0;

	printf_dbg("parsed uri: %s\n", request->uri);
	return buffer + uri_length + 1;
}

/*
 * TODO set errno to distinguish between a failed memory allocation (server error) and parsing error (bad request)
 */
char *parse_http_version(char *buffer, http_request *request){
	char *start_of_buffer = buffer;
	char buffer_char_backup = 0; //used for debugging purposes
	if (strncmp(buffer, HTTP_VERSION_PREFIX, sizeof(HTTP_VERSION_PREFIX) - 1)){
		swap(&buffer[sizeof(HTTP_VERSION_PREFIX)], &buffer_char_backup);
		printf_dbg("failed to parse prefix, got: \n", buffer);
		swap(&buffer[sizeof(HTTP_VERSION_PREFIX)], &buffer_char_backup);
		return 0;
	}
	buffer += sizeof(HTTP_VERSION_PREFIX) - 1;
	char *expected_end_of_number_position = strchr(buffer, '.');
	if(expected_end_of_number_position == 0){
		swap(expected_end_of_number_position, &buffer_char_backup);
		printf_dbg("failed to find dot, got: \n", buffer);
		swap(expected_end_of_number_position, &buffer_char_backup);
		return 0;
	}

	char *endptr;
	strtol(buffer, &endptr, 10);
	if (endptr != expected_end_of_number_position){
		swap(expected_end_of_number_position, &buffer_char_backup);
		printf_dbg("failed to parse major version, got: \n", buffer);
		swap(expected_end_of_number_position, &buffer_char_backup);
		return 0;
	}

	buffer = expected_end_of_number_position + 1; // go past the dot

	expected_end_of_number_position = strstr(buffer, CRLF);
	if(expected_end_of_number_position == 0){
		swap(expected_end_of_number_position, &buffer_char_backup);
		printf_dbg("failed to parse CRLF version, got: \n", buffer);
		swap(expected_end_of_number_position, &buffer_char_backup);
		return 0;
	}
	
	strtol(buffer, &endptr, 10);
	if(*buffer == 'x')
		endptr = expected_end_of_number_position;
	if(endptr != expected_end_of_number_position){
		swap(expected_end_of_number_position, &buffer_char_backup);
		printf_dbg("failed to parse minor version, got: \n", buffer);
		swap(expected_end_of_number_position, &buffer_char_backup);
		return 0;
	}

	buffer = expected_end_of_number_position;

	if(strncmp(buffer, CRLF, sizeof(CRLF) - 1)){
		printf_dbg("expected CRLF\n");
		return 0;
	}

	request->http_version = strndup(start_of_buffer, buffer - start_of_buffer);
	if(request->http_version == 0){
		printf_dbg("failed to strndup\n");
		return 0;
	}

	printf_dbg("parsed http_version: %s\n", request->http_version);
	return buffer;
}

char *parse_headers(char *buffer, http_message *message){
	while(strncmp(buffer, CRLF, sizeof(CRLF) - 1)){
		buffer = parse_next_http_header(buffer, message);
		if(buffer == 0)
			return 0;
		if(strncmp(buffer, CRLF, sizeof(CRLF) - 1))
			return 0;
		buffer += sizeof(CRLF) - 1;
	}
	return buffer;
}
/*
 * TODO support space at the start of line (multi line header)
 * the returned offset is expected to be behind a CRLF
 */
char *parse_next_http_header(char *buffer, http_message *message){
	char *end_of_token = strchr(buffer, ':');
	char buffer_char_backup = 0;
	if(end_of_token == 0){
		printf_dbg("didn't find column\n");
		errno = EINVAL;
		return 0;
	}

	swap(end_of_token, &buffer_char_backup);
	http_header header;
	enum HTTP_HEADER_NAME_ENUM header_name = http_header_str2enum(buffer);
	swap(end_of_token, &buffer_char_backup);
	if (header_name == UNSPEC_HEADER){
		printf_dbg("header name not supported, placing UNSPEC_HEADER\n");
	}
	buffer = end_of_token + 1; // one to go past the column

	buffer = skip_all_consecutive_char(buffer, ' '); // skip leading spaces
	end_of_token = skip_until_trailing_whitespace_or_cr(buffer); // position of start of trailing spaces or CRLF
	if(end_of_token == 0){
		printf_dbg("unable to find end of field value\n");
		errno = EINVAL;
		return 0;
	}

	swap(end_of_token, &buffer_char_backup);
	add_header_to_http_message(message, header_name, buffer);
	swap(end_of_token, &buffer_char_backup);
	buffer = end_of_token;
	buffer = skip_all_consecutive_char(buffer, ' ');

	if(strncmp(buffer, CRLF, sizeof(CRLF) - 1)){
		printf_dbg("expected CRLF, got %s\n", buffer);
		errno = EINVAL;
		return 0;
	}

	return buffer;
}

char *parse_body(char *buffer, http_message *message){
	http_request *request = (http_request *)message;
	request->body = 0;
	return buffer;
}

void display_http_request(http_request *request){
	printf("%s %s %s\n", 
			http_method2str[request->method],
			request->uri,
			request->http_version
			);

	http_header *current_header = request->headers;
	while(current_header->header_name != NULL_HEADER){
		printf("%s: %s\n", 
				http_header_enum2str[current_header->header_name],
				current_header->field_value
				);
		current_header++;
	}
	putchar('\n');

	if(request->body)
		printf("%s", request->body);
}
void display_http_response(http_response *response){
	printf("%s %d %s\n", 
			response->http_version,
			http_status_enum2struct[response->status_code].code,
			http_status_enum2struct[response->status_code].phrase
			);

	http_header *current_header = response->headers;
	while(current_header->header_name != NULL_HEADER){
		printf("%s: %s\n", 
				http_header_enum2str[current_header->header_name],
				current_header->field_value
				);
		current_header++;
	}
	putchar('\n');

	printf("body: ");
	switch (response->body_type) {
		case (FILE_PTR): {
					 printf("%d\n", fileno(response->body.file));
					 break;
				 };
		case(BUFFER): {printf("%s\n", response->body.buffer);
				      break;
			      };
		case(NONE): {printf("none\n");
				    break;
			    };
	}
}
