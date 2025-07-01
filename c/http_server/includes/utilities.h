#ifndef UTILS 
#define UTILS
#include <stdio.h>
#include <stdbool.h>
void if_err_log_n_quit(bool);
size_t file_size(FILE *);
char *file_size_str(FILE *);
void swap (char *, char *);
char *skip_all_consecutive_char(char *, char);


/*
 * this function stops at '\r' or '\0'
 * if the buffer doesn't have trailing white space or cr, it will stop at trailing null byte
 */
char *skip_until_trailing_whitespace_or_cr(char *);
#endif
