#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define MAX_CONNECTIONS 10
#define PACKET_LENGTH 1400

int sans_connect(const char* hostname, int port, int protocol) {
  if(protocol != IPPROTO_TCP){
    return -1;
  }
  struct addrinfo connection_hints, *connection_info, *curr_info;
  char portBuffer[12];
  int socket_fd = -1;

  memset(&connection_hints, 0, sizeof(connection_hints));
  connection_hints.ai_family = AF_UNSPEC; 
  connection_hints.ai_socktype = SOCK_STREAM; 

  snprintf(portBuffer, 11, "%d", port);
  getaddrinfo(hostname, portBuffer, &connection_hints, &connection_info);

  for(curr_info = connection_info; curr_info != NULL; curr_info = curr_info->ai_next){
    socket_fd = socket(curr_info->ai_family, curr_info->ai_socktype, curr_info->ai_protocol);
    if(socket_fd == -1) continue;
    if(connect(socket_fd, curr_info->ai_addr, curr_info->ai_addrlen) != -1){
        break;
    }
    close(socket_fd);
  }
  return socket_fd;
}

int sans_accept(const char* address, int port, int protocol) {
    if(protocol != IPPROTO_TCP){
        return -1;
    }
    struct addrinfo connection_hints, *connection_info, *curr_info;
    int server_socket_f_descriptor = -1, client_socket_f_descriptor;
    char portBuffer[12];

    snprintf(portBuffer, 11, "%d", port);

    memset(&connection_hints, 0, sizeof connection_hints);
    connection_hints.ai_family = AF_UNSPEC;
    connection_hints.ai_socktype = SOCK_STREAM;
    connection_hints.ai_flags = AI_PASSIVE;

    getaddrinfo(address, portBuffer, &connection_hints, &connection_info);

    for (curr_info = connection_info; curr_info != NULL; curr_info = curr_info->ai_next) {
        server_socket_f_descriptor = socket(curr_info->ai_family, curr_info->ai_socktype, curr_info->ai_protocol);
        if (server_socket_f_descriptor == -1) continue;

        int reuse_option = 1;
        setsockopt(server_socket_f_descriptor, SOL_SOCKET, SO_REUSEADDR, &reuse_option, sizeof(reuse_option));

        if (bind(server_socket_f_descriptor, curr_info->ai_addr, curr_info->ai_addrlen) == 0) {
            if (listen(server_socket_f_descriptor, MAX_CONNECTIONS) == -1) {
                close(server_socket_f_descriptor);
                continue;
            }
            break; 
        }
        close(server_socket_f_descriptor); 
    }

    client_socket_f_descriptor = accept(server_socket_f_descriptor, NULL, NULL);
    if (client_socket_f_descriptor == -1) {
        close(server_socket_f_descriptor);
        return -1; 
    }
    return client_socket_f_descriptor; 
}

int sans_disconnect(int socket_fd) {
  return close(socket_fd);
}
