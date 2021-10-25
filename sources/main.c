#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

struct IpAddress
{
    uint8_t byte0;
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;
};

int main(int argc, char **argv)
{
    int port;
    int listening_socket;
    int serving_socket;
    int c;
    struct sockaddr_in server_endpoint;
    struct sockaddr_in client_endpoint;

    if (argc != 2)
    {
        printf("port argument missing");
        return 1;
    }
    port = atoi(argv[1]);
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == -1)
    {
        printf("creation error");
        return 1;
    }
    else
    {
        server_endpoint.sin_family = AF_INET;
        server_endpoint.sin_addr.s_addr = INADDR_ANY;
        server_endpoint.sin_port = htons((u_int16_t)port);
        if (bind(listening_socket, (struct sockaddr *)&server_endpoint, sizeof(server_endpoint)) < 0)
        {
            printf("bind error\n");
            return 1;
        }
        else
        {
            printf("bind done\n");
            listen(listening_socket, 3);
            printf("Waiting for incoming connections...\n");
            c = sizeof(struct sockaddr_in);
            while ((serving_socket = accept(listening_socket, (struct sockaddr *)&client_endpoint, (socklen_t *)&c)))
            {
                struct IpAddress *ip = (struct IpAddress *)(&client_endpoint.sin_addr.s_addr);
                char *buffer = malloc(1024);
                char *message = malloc(1024);
                memset(buffer, 0, 1024);
                memset(message, 0, 1024);

                recv(serving_socket, buffer, 1024, 0);
                sprintf(message, "hello %d.%d.%d.%d:%hu this is your echo '%s'", ip->byte0, ip->byte1, ip->byte2, ip->byte3, client_endpoint.sin_port, buffer);
                free(buffer);

                write(serving_socket, message, strlen(message));
                printf("%s\n", message);
                free(message);
            }
            if (serving_socket < 0)
            {
                printf("serving error\n");
                return 1;
            }
        }
    }
    return 0;
}
