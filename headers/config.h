#ifndef HEADER_CONFIG
#define HEADER_CONFIG

struct Configuration
{
    char Port[256];
};

int search_environment_variable(char *name, char *buffer);
int search_config_path(char *buffer, char *environment_variable);
int check_config_file_read_access(char *buffer);
void read_config_file_line(char *line, char *label, char *value);
int read_config_file(char *config_path, struct Configuration *configuration);
struct Configuration *allocate_config(void);
int load_config(char *environment_variable, struct Configuration *configuration);
void print_config(struct Configuration *configuration);

#endif
