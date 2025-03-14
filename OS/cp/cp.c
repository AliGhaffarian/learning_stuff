#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#define INVALID_ARGS_MSG "invalid args"
#define FD_ARR_SIZE 2
#define OPEN_FILE_ERR_MSG "error during openning file"

#define DEBUG_PREFIX "DEBUG : "
#define VERBOSE_PREFIX "VERBOSE : "


int do_create = 0;
char do_create_help[] = "-c : create destination file if doesn't exist";

int block_size = 128;
char block_size_help[] = "-b [SIZE] : size of transfer buffer bigger block size typically means faster transfer of data";

int do_debug = 0;
char do_debug_help[] = "-d :  print debug logs";

int do_verbose = 0;
char do_verbose_help[] = "-v : not implemented";

int overwrite = 0;
char overwrite_help[] = "-f : overwrite destination file without asking";

int creat_chmod_mask = S_IRUSR | S_IRGRP | S_IROTH;

char* src_path = NULL;
char* dst_path = NULL;

/*
   options to do:
   */
void finish_and_exit(int* fd_to_close){
		int i = 0;
		while(i < FD_ARR_SIZE){
				close(fd_to_close[i++]);
		}
		exit(errno);
}

int read_for_confirm(){
		char input;
		int read_bytes = read(STDIN_FILENO, &input, 1);
		if(read_bytes == -1){
				printf("failed to read inout\n");
				exit(1);
		}
		if(input == 'y' || input == 'Y')
				return 1;
		return 0;
}

void init_args(int argc, char** argv){
		int opt;
		while((opt = getopt(argc, argv, "hcdvfb:")) != -1){
				switch(opt){
						case 'd':{
										 do_debug = 1;
										 break;
								 }
						case 'v':{
										 do_verbose = 1;
										 break;
								 }
						case 'b':{
										 block_size = atoi(optarg);
										 break;
								 }
						case 'f':{
										 overwrite = 1;
										 break;
								 }

						case 'm':{
										 //will get to this later
										 creat_chmod_mask = atoi(optarg);
										 break;

								 }
						case 'c':{
										 do_create = 1;
										 break;
								 }
						case 'h':{
										 printf("usage : %s [OPTIONS] source_file_path destination_file_path\n", argv[0]);
										 printf("%s\n", do_debug_help);
										 printf("%s\n", do_verbose_help);
										 printf("%s\n", block_size_help);
										 printf("%s\n", overwrite_help);
										 printf("%s\n", do_create_help);
										 exit(1);
								 }
						default:break;
				}
		}

		src_path = argv[argc - 2];
		dst_path = argv[argc - 1];

		if (do_debug){
				printf("%sdebug flag : %d\n", DEBUG_PREFIX ,do_debug);
				printf("%sverbose flag : %d\n", DEBUG_PREFIX, do_verbose);
				printf("%soverwrite flag : %d\n", DEBUG_PREFIX, overwrite);
				printf("%sblock size : %d\n", DEBUG_PREFIX, block_size);
				printf("%ssource path : %s\n", DEBUG_PREFIX, src_path);
				printf("%sdestination path : %s\n", DEBUG_PREFIX, dst_path);
				printf("%schmod perms for created files : %d\n", DEBUG_PREFIX, creat_chmod_mask);
				printf("%screate destination file if doesnt exist : %d\n", DEBUG_PREFIX, do_create);
		}
} 

int main(int argc, char** argv){
		init_args(argc, argv);
		char* paths[FD_ARR_SIZE];
		int fd_list[FD_ARR_SIZE];

		int src = open(src_path, O_RDONLY);
		if(src == -1){
				printf("%s", OPEN_FILE_ERR_MSG);
				printf(": %s\n", src_path);
				finish_and_exit(fd_list);
		}

		fd_list[0] = src;

		int dst = open(dst_path, O_WRONLY);
		if (dst != -1){
				if (overwrite == 0){
						printf("%s exists overwrite? y/N\n", dst_path);

						if(read_for_confirm() == 0){
								printf("aborting\n");
								exit(1);
						}
				}
		}
		else if (do_create){
				dst = open(dst_path, O_WRONLY | O_CREAT);
				chmod(dst_path, creat_chmod_mask);
		}

		//unexpected error
		if(dst == -1){
				printf("%s", OPEN_FILE_ERR_MSG);
				printf(": %s\n", dst_path);
				finish_and_exit(fd_list);
		}


		fd_list[1] = dst;

		char* buff = (char*)malloc(sizeof(char) * block_size);
		if(buff == 0){
				printf("failed to allocate %s bytes for the transfer buffer\n");
				exit(1);
		}
		int read_bytes = 0;
		int write_bytes = 0;
		int total_size = 0;
		while (1){
				read_bytes = read( src, buff, block_size );
				if(read_bytes == -1){
						printf("err during reading from %s\n", src_path);
						finish_and_exit(fd_list);
				}

				write_bytes = write( dst, buff, read_bytes);
				total_size += write_bytes;
				if(write_bytes == -1){
						printf("err during writing to %s\n", dst_path);
						finish_and_exit(fd_list);
				}

				if(write_bytes !=  read_bytes){
						printf("read and write size mismatch read : %d write : %d\n", read_bytes, write_bytes);
						exit(1);
				}

				if ( do_debug ) printf("transfered %d bytes\n", write_bytes);

				if (read_bytes != block_size || read_bytes == 0){
						printf("total transfered data : %d bytes\n", total_size);
						finish_and_exit(fd_list);
				}

		}

}
