#include <stdlib.h>
#include <arpa/inet.h>
#include <server.h>
#include "meta.h"

int main(int argc, char **argv)
{
    char *argument = get_first_argument(argc, argv);
    int port = atoi(argument);
    int socket_fd = get_socket();
    struct sockaddr_in socket_config = configure_socket(port);
    bind_socket(socket_fd, socket_config);
    serve(socket_fd);
    return 0;
}
