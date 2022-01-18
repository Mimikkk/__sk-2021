#include <shared/utils/error_handle.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "server.h"

static const int MaxConnections = 20;
static const int Port = 9090;
static const char Address[] = "127.0.0.1";

static int server_socket;
static struct sockaddr_in server_address;

static void initialize_socket(void) {
  server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  let had_error = server_socket < 0;
  quit.on(had_error, "Failed to create_epoll a socket");
}
static void initialize_reusable() {
  let had_error = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &(int) {1}, sizeof(int)) < 0;
  quit.on(had_error, "Failed to set socket reusable");
}
static void bind_server(void) {
  let had_error = bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0;
  quit.on(had_error, "Failed to bind socket");
}
static void start_listening(void) {
  let had_error = listen(server_socket, MaxConnections) < 0;
  quit.on(had_error, "server refused to listen");
}
static void initialize_address(void) {
  server_address = (struct sockaddr_in) {AF_INET, htons(Port), {inet_addr(Address)}};
}

static void open_server(void) {
  initialize_socket();
  initialize_reusable();
  initialize_address();
  bind_server();
  start_listening();
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
