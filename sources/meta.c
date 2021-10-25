#include "meta.h"
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int exit_with_details(int code, char *custom_message)
{
    int err = errno;
    char *err_message = strerror(err);
    printf("%s, error code %d, %s\n", custom_message, err, err_message);
    exit(code);
}

void *malloc_memset(size_t size, int set)
{
    char *mem = malloc(size);
    memset(mem, set, size);
    return mem;
}
