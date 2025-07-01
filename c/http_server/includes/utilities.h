#ifndef UTILS 
#define UTILS
#include <stdio.h>
#include <stdbool.h>
void if_err_log_n_quit(bool);
size_t file_size(FILE *);
char *file_size_str(FILE *);
void swap (char *, char *);
char *skip_all_consecutive_char(char *, char);
char *skip_until_trailing_whitespace_or_cr(char *);
#endif
