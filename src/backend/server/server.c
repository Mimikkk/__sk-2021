#include <shared/utils/error_handle.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "server.h"

static const int MaxConnections = 3;
static const int Port = 9090;
static const char Address[] = "127.0.0.1";

static int server_socket;
static struct sockaddr_in server_address;

static int set_socket(void) {
  return server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
}
static int set_reusable(void) {
  return setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &(int) {1}, sizeof(int));
}
static int set_bind(void) {
  return bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
}
static int start_listening(void) {
  return listen(server_socket, MaxConnections);
}
static void set_address(void) {
  server_address = (struct sockaddr_in) {AF_INET, htons(Port), {inet_addr(Address)}};
}

static void open_server(void) {
  quit.on(set_socket() < 0, "Failed to create a socket");
  quit.on(set_reusable() < 0, "Failed to set socket's options");
  set_address();
  quit.on(set_bind() < 0, "server refused to bind");
  quit.on(start_listening() < 0, "server refused to listen");
}
static void close_server(void) {
  close(server_socket);
}
static const char *server_info(void);

static int accept_connection(void) {
  int connection_socket = accept(*server.socket, NULL, NULL);
  fcntl(connection_socket, F_SETFL, O_NONBLOCK, true);
  return connection_socket;
}

const struct server_lib server = {
        .close = close_server,
        .open = open_server,
        .info = server_info,
        .accept = accept_connection,

        .port = Port,
        .socket = &server_socket,
        .address = &server_address,
        .max_connections = MaxConnections,
};

static const char *server_info(void) {
  return str("socket '%d' at %s:%d with %d max connections",
             *server.socket, inet_ntoa(server.address->sin_addr), server.port, server.max_connections);
}
