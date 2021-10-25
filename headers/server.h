#ifndef HEADER_SERVER
#define HEADER_SERVER

int get_socket(void);
struct sockaddr_in configure_socket(int port);
int bind_socket(int socket, struct sockaddr_in config);
void serve(int socket_fd);
void receive(int socket, char *buffer);
void respond(int client_socket, char *output_buffer);

#endif
