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

int get_socket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        exit_with_details(1, "socket creation error");
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
        exit_with_details(1, "socket bind error");
    return status;
}

void handle_peer_concurrent(int socket_fd, struct sockaddr_in peer_endpoint_meta_infos)
{
    int process_id = fork();
    if (process_id == 0)
    {
        handle_peer(socket_fd, peer_endpoint_meta_infos);
        _exit(0);
    }
    else if (process_id < 0)
        exit_with_details(1, "process fork error");
}

void handle_peer(int socket_fd, struct sockaddr_in peer_endpoint_meta_infos)
{
    struct IpAddress *ip_address = (struct IpAddress *)&(peer_endpoint_meta_infos.sin_addr.s_addr);
    char *input_buffer = malloc_memset(BUFFER_SIZE, 0);
    char *output_buffer = malloc_memset(BUFFER_SIZE, 0);

    receive(socket_fd, input_buffer);
    process(input_buffer, output_buffer);
    respond(socket_fd, output_buffer);

    printf(
        "served %d.%d.%d.%d:%hu | request '%s' | response '%s'\n",
        ip_address->byte0,
        ip_address->byte1,
        ip_address->byte2,
        ip_address->byte3,
        peer_endpoint_meta_infos.sin_port,
        input_buffer,
        output_buffer);

    free(output_buffer);
    free(input_buffer);
}

void serve(int listening_socket_fd)
{
    struct sockaddr_in peer_endpoint_meta_infos;
    struct sockaddr *peer_endpoint_meta = (struct sockaddr *)&peer_endpoint_meta_infos;
    socklen_t size_of_meta_infos = sizeof(*peer_endpoint_meta);
    int serving_socket_fd;

    listen(listening_socket_fd, 3);

    while ((serving_socket_fd = accept(listening_socket_fd, peer_endpoint_meta, &size_of_meta_infos)))
        handle_peer_concurrent(serving_socket_fd, peer_endpoint_meta_infos);

    if (serving_socket_fd < 0)
        exit_with_details(1, "accept error");
}

void receive(int socket, char *buffer)
{
    ssize_t status = recv(socket, buffer, BUFFER_SIZE, 0);
    if (status < 0)
        exit_with_details(1, "request receive error");
}

void respond(int client_socket, char *output_buffer)
{
    unsigned long payload_length = strlen(output_buffer);
    ssize_t status = write(client_socket, output_buffer, payload_length);
    if (status < 0)
        exit_with_details(1, "responds write error");
}
