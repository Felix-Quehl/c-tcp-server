#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define STR_FMT_CONFIG_ERROR "failed to get configuration file path from environment variable '%s': %s\n"
#define STR_FMT_CONFIG_READ_ERROR "configuration file read test failed: '%s'\n"
#define HARD_CODE_STRING(Variable) (#Variable)
#define NEW_LINE '\n'
#define DELIMITER '='
#define COMMENT '#'
#define READ_MODE "r"

int search_environment_variable(char *name, char *buffer)
{
    int status_code = 0;
    char *temp;
    temp = getenv(name);
    if (temp == NULL)
        status_code = EIDRM;
    else
        strcpy(buffer, temp);
    return status_code;
}

int search_config_path(char *buffer, char *environment_variable)
{
    int status_code = 0;
    status_code = search_environment_variable(environment_variable, buffer);
    return status_code;
}

int check_config_file_read_access(char *buffer)
{
    int status_code = 0;
    status_code = access(buffer, R_OK);
    if (status_code == -1)
        status_code = ENOENT;
    return status_code;
}

void read_config_file_line(char *line, char *label, char *value)
{
    if (*line != COMMENT)
    {
        size_t line_length = strlen(line);
        char *delimiter = strchr(line, DELIMITER);
        if (delimiter != NULL)
        {
            int offset = 1;
            if (line[line_length - 1] == NEW_LINE)
                offset = 2;
            strncpy(label, line, (size_t)(delimiter - line));
            strncpy(value, delimiter + 1, (size_t)(line + line_length - offset - delimiter));
        }
    }
}

int read_config_file(char *config_path, struct Configuration *configuration)
{
    int status_code = 0;
    FILE *filePointer = fopen(config_path, READ_MODE);
    if (filePointer != NULL)
    {
        char line[BUFFER_SIZE] = {0};
        while (!feof(filePointer))
        {

            char label[BUFFER_SIZE] = {0};
            char value[BUFFER_SIZE] = {0};
            memset(line, 0, BUFFER_SIZE);

            fgets(line, BUFFER_SIZE, filePointer);
            read_config_file_line(line, label, value);

            if (!strcmp(label, HARD_CODE_STRING(configuration->Port)))
                strcpy(configuration->Port, value);
        }
        fclose(filePointer);
    }
    else
        status_code = EBADFD;
    return status_code;
}

struct Configuration *allocate_config()
{
    struct Configuration *configuration;
    configuration = malloc(sizeof(*configuration));
    memset(configuration, 0, sizeof(*configuration));
    return configuration;
}

int load_config(char *environment_variable, struct Configuration *configuration)
{
    int status_code = 0;
    char config_path[BUFFER_SIZE] = {0};
    status_code = search_config_path(config_path, environment_variable);
    if (status_code == 0)
    {
        status_code = check_config_file_read_access(config_path);
        if (status_code == 0)
            status_code = read_config_file(config_path, configuration);
        else
            printf(STR_FMT_CONFIG_READ_ERROR, strerror(status_code));
    }
    else
        printf(STR_FMT_CONFIG_ERROR, environment_variable, strerror(status_code));
    return status_code;
}

void print_config(struct Configuration *configuration)
{
    printf("CONFIG________________________________\n");
    printf(HARD_CODE_STRING(configuration->Port));
    printf(" = %s\n", configuration->Port);
    printf("--------------------------------------\n");
}
