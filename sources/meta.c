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

char *get_first_argument(int argc, char **argv)
{
    if (argc < 2)
        exit_with_details(1, "missing arguments");
    return argv[1];
}
