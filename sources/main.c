#include <stdlib.h>
#include <arpa/inet.h>
#include <server.h>
#include "meta.h"
#include "config.h"

#define ENV_VAR_CONFIG "CONFIG"

int main()
{
    struct Configuration *configuration;
    int status_code;
    int port;
    int socket_fd;
    struct sockaddr_in socket_config;

    configuration = allocate_config();
    status_code = load_config(ENV_VAR_CONFIG, configuration);

    if (status_code == 0)
        print_config(configuration);

    port = atoi(configuration->Port);
    socket_fd = get_socket();
    socket_config = configure_socket(port);
    bind_socket(socket_fd, socket_config);
    serve(socket_fd);
    return 0;
}
