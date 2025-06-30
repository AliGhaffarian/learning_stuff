#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <http.h>
#include <http_server.h>
#include <utilities.h>
#include <debug.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

int busy_threads = 0;
pthread_mutex_t busy_threads_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t update_on_busy_threads_cond = PTHREAD_COND_INITIALIZER;

void handle_badrequest(int fd, http_request request){
	printf_dbg("called\n");
	return;
}
void handle_server_err(int fd){
	printf_dbg("called\n");
	send(fd, HTTP_SERVER_ERROR, sizeof(HTTP_SERVER_ERROR) - 1, 0);
	close(fd);
	return;
}
void handle_open_err(int fd){
	/*
	 * based on the errno send the client err msg
	 * */
	return handle_server_err(fd);
}

void send_http_response(int fd, http_response response){
	printf_dbg("sending http response:");
#ifdef DEBUG
	display_http_response(&response);
#endif

	char send_buffer[BUFSIZ];
	int printed_bytes = sprintf(
			send_buffer,"%s %d %s%s", 
			response.http_version,
			http_status_enum2struct[response.status_code].code, 
			http_status_enum2struct[response.status_code].phrase,
			CRLF
			);
	send(fd, send_buffer, printed_bytes, 0); 

	http_header *current_header = response.headers;
	while(current_header->header_name != NULL_HEADER){
		printed_bytes = sprintf(
				send_buffer,
				"%s: %s%s",
				http_header_enum2str[current_header->header_name],
				current_header->field_value,
				CRLF
				);
				 
		send(fd, send_buffer, printed_bytes, 0); 

		current_header++;
	}
	send(fd, CRLF, sizeof(CRLF) - 1, 0);
	switch (response.body_type) {
		case FILE_PTR:{
				      int body_file_fd = fileno(response.body.file);
				      if (fd == -1){
					      free_http_message((http_message *)&response);
					      return handle_server_err(fd);
				      }
				      
				      sendfile(fd, body_file_fd, 0, file_size(response.body.file));
				      break;
			      }
		case BUFFER:{
				    send(fd, response.body.buffer, strlen(response.body.buffer), 0);
				    break;
			    }
		case NONE:{
				  break;
			  }
		default:{
				free_http_message((http_message *)&response);
				return handle_server_err(fd);
			}
	}
	send(fd, END_OF_HTTP_MESSAGE, sizeof(END_OF_HTTP_MESSAGE), 0);

	free_http_message((http_message *)&response);
	close(fd);
}

void handle_head(int fd, http_request request){
	return;
}
void handle_get(int fd, http_request request){
	FILE *requested_file = fopen(request.uri, "r");
	if (requested_file == 0){
		printf_dbg("error on open %s: %s\n", request.uri, strerror(errno));
		free_http_message((http_message *)&request);
		return handle_open_err(fd);
	}

	http_response response;
	int err = make_http_response(&response);
	if(err)
		return handle_server_err(fd);
	response.status_code = STATUS_200_SUCCESS;
	response.http_version = strdup(request.http_version);
	response.body_type = FILE_PTR;
	response.body.file = requested_file;

	if (response.http_version == 0){
		free_http_message((http_message *)&request);
		return handle_server_err(fd);
	}


	bool success = add_header_to_http_message(
			(http_message *)&response,
			CONTENT_LENGTH,
			file_size_str(requested_file)
			);
	printf("response after adding header:");

	if (success == false){
		free_http_message((http_message *)&request);
		return handle_server_err(fd);
	}


	free_http_message((http_message *)&request);
	return send_http_response(fd, response);
}

char *decode_percenthex_chars(char *buf){
	int buflen = strlen(buf);
	char *result = malloc(buflen);
	memcpy(result, buf, buflen);
	return result;
}

void (*http_handlers[])(int, http_request) = {
	[HTTP_GET] = handle_get,
	[HTTP_HEAD] = handle_head,
	[HTTP_BAD_REQUEST] = handle_badrequest
};


void skip_initial_crlfs(int fd, char *buf, int *current_offset){
	return;
}

int recv_and_parse_http_request(int fd, http_request *request){

	char *recv_buffer = malloc(HTTP_BUFFER_SIZE);
	char *beginning_of_buffer = recv_buffer;
	int err = 0;
	if(recv_buffer == 0){
		err = -1;
		printf_dbg("error on malloc: %s\n", strerror(errno));
		goto free_msg_cleanup;
	}

	printf_dbg("recving\n");
	int bytes_recved = recv(fd, recv_buffer, HTTP_BUFFER_SIZE - 1, 0);
	if (bytes_recved == -1){
		err = -1; 
		printf_dbg("error on recv: %s\n", strerror(errno));
		goto free_buff_and_msg_cleanup;
	}	
	recv_buffer[bytes_recved] = 0; //add null byte
	printf_dbg("recieved message:\n");
	printf_dbg("%s\n", recv_buffer);

	recv_buffer = parse_http_request(recv_buffer, (http_message *)request);
	if(recv_buffer == 0){
		err = -1;
		printf_dbg("error on parsing\n");
		goto free_buff_and_msg_cleanup;
	}

	return err;

free_buff_and_msg_cleanup:
	printf_dbg("went to free_buff_and_msg_cleanup\n");
	free(beginning_of_buffer);
free_msg_cleanup:
	printf_dbg("went to free_msg_cleanup\n");
	free_http_message((http_message *)request);

	return err;
}

void handle_client(int fd){
	http_request request = {0};
	int err = make_http_request(&request);
	if (err)
		return handle_server_err(fd);
	err = recv_and_parse_http_request(fd, &request);
	printf_dbg("err of recv_and_parse_http_request = %d\n", err);
	if (err)
		return handle_server_err(fd); //TODO distinuish between err values for handle_badrequest, handle_server_err, handle_404
					      
	printf_dbg("calling handler for method: %s\n", http_method2str[request.method]);
	http_handlers[request.method](fd, request);
}

void *handle_client_wrapper(void *void_arg){
	printf("im %p\n", pthread_self());

	int fd = (int)void_arg;
	handle_client(fd);

	pthread_mutex_lock(&busy_threads_mutex);
	busy_threads--;
	pthread_cond_signal(&update_on_busy_threads_cond);
	pthread_mutex_unlock(&busy_threads_mutex);
	
	pthread_exit(0);
}

void run(struct sockaddr_in socket_address, int n_threads, int queue_length){
	busy_threads = 0;
	pthread_t handle_client_thread[21];
	pthread_attr_t handle_client_t_attr;
	pthread_attr_init(&handle_client_t_attr);
	pthread_attr_setdetachstate(&handle_client_t_attr, PTHREAD_CREATE_DETACHED);
	struct sockaddr_in client_addr;
	socklen_t client_addrlen;

	int sock = socket(socket_address.sin_family, SOCK_STREAM, 0);
	int reuse = 1;
	int err = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
	if(err){
		printf_dbg("faild to set socket option: %s\n", strerror(errno));
	}

	err = bind(
			sock, 
			(struct sockaddr *)&socket_address, 
			sizeof(socket_address)
		      );
	if_err_log_n_quit(err == -1);

	err = listen(sock, queue_length);

	busy_threads = 0;
	while(1){
		pthread_mutex_lock(&busy_threads_mutex);
		while(busy_threads >= n_threads){
			pthread_cond_wait(&update_on_busy_threads_cond, &busy_threads_mutex);
		}
		pthread_mutex_unlock(&busy_threads_mutex);
		
		
		int connect_sock = accept(sock, 
				(struct sockaddr *)&client_addr, 
				&client_addrlen);

		printf_dbg("accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		pthread_create(&handle_client_thread[busy_threads], 
				&handle_client_t_attr, 
				handle_client_wrapper,
				(void*)connect_sock);

		pthread_mutex_lock(&busy_threads_mutex);
		busy_threads++;
		printf("adding one to busy_threads: %d\n", busy_threads);
		pthread_mutex_unlock(&busy_threads_mutex);
	
	}
}
