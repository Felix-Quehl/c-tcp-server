#ifndef HEADER_META
#define HEADER_META

#include <stdlib.h>

int exit_with_details(int code, char *custom_message);
char *get_first_argument(int argc, char **argv);
void *malloc_memset(size_t size, int set);

#endif
