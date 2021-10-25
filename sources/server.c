#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <server.h>
#include <meta.h>
#include <logic.h>

#define BUFFER_SIZE 10240

struct IpAddress
{
    uint8_t byte0;
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;
};

void serve(int listening_socket_fd)
{
    struct sockaddr_in peer_endpoint_meta_infos;
    struct sockaddr *peer_endpoint_meta = (struct sockaddr *)&peer_endpoint_meta_infos;
    struct IpAddress *ip_address = (struct IpAddress *)&(peer_endpoint_meta_infos.sin_addr.s_addr);
    socklen_t c = sizeof(struct sockaddr_in);
    int serving_socket_fd;

    listen(listening_socket_fd, 3);

    while ((serving_socket_fd = accept(listening_socket_fd, peer_endpoint_meta, &c)))
    {
        char *input_buffer = malloc(BUFFER_SIZE);
        char *output_buffer = malloc(BUFFER_SIZE);
        memset(input_buffer, 0, BUFFER_SIZE);
        memset(output_buffer, 0, BUFFER_SIZE);

        receive(serving_socket_fd, input_buffer);
        process(input_buffer, output_buffer);
        respond(serving_socket_fd, output_buffer);

        printf(
            "served %d.%d.%d.%d:%hu | request '%s' | response '%s'\n",
            ip_address->byte0,
            ip_address->byte1,
            ip_address->byte2,
            ip_address->byte3,
            peer_endpoint_meta_infos.sin_port,
            input_buffer,
            output_buffer);

        free(input_buffer);
        free(output_buffer);
    }
    if (serving_socket_fd < 0)
        exit_with_details(1, "serving failed");
}

int get_socket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        exit_with_details(1, "failed to create socket");
    return fd;
}

struct sockaddr_in configure_socket(int port)
{
    struct sockaddr_in server_endpoint;
    server_endpoint.sin_family = AF_INET;
    server_endpoint.sin_addr.s_addr = INADDR_ANY;
    server_endpoint.sin_port = htons((u_int16_t)port);
    return server_endpoint;
}

int bind_socket(int socket, struct sockaddr_in config)
{
    int status = bind(socket, (struct sockaddr *)&config, sizeof(config));
    if (status < 0)
        exit_with_details(1, "socket bind failed");
    return status;
}
void receive(int socket, char *buffer)
{
    ssize_t status = recv(socket, buffer, BUFFER_SIZE, 0);
    if (status < 0)
        exit_with_details(1, "write error");
}

void respond(int client_socket, char *output_buffer)
{
    unsigned long payload_length = strlen(output_buffer);
    ssize_t status = write(client_socket, output_buffer, payload_length);
    if (status < 0)
        exit_with_details(1, "write error");
}
