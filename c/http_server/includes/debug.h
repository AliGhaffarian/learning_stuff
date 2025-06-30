#ifndef DEBUGGING_HEADER
#define DEBUGGING_HEADER

#ifdef DEBUG
#include <stdio.h>
#define print_debug_prefix printf("[DEBUG: %s] ", __func__);
#define printf_dbg print_debug_prefix; printf
#else
#define printf_dbg(...)
#endif

#endif
