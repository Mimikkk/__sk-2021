#include <shared/utils/console.h>
#include <shared/utils/error_handle.h>
#include <shared/utils/imports.h>
#include <shared/utils/common.h>

static const int MaxConnections = 3;
static const int MaxEvents = 10;

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

static void create_server(void) {
  quit.on(set_socket() < 0, "Failed to create socket");
  quit.on(set_reusable() < 0, "Failed to set socket options");
  set_address();
  quit.on(set_bind() < 0, "server refused to bind");
  quit.on(start_listening() < 0, "server refused to listen");
}
static void close_server(void) {
  close(server_socket);
}

int main(void) {
  console.info("Starting server...");
  create_server();
  console.info("Started server.");

  console.info("Waiting for clients...");
  console.info("Waiting for events...");

  console.info("Client 1 connecting...");
  console.info("Client 1 connected.");
  console.info("Current users 1.");
  console.info("Client 2 connecting...");
  console.info("Client 2 connected.");
  console.info("Current users 2.");
  console.info("Client 3 connecting...");
  console.info("Client 3 connected.");
  console.info("Current users 3.");

  console.info("Client 4 connecting...");
  console.info("Client 4 refused ( Max users reached ).");

  console.info("Client 3 disconnected.");
  console.info("Current users 2.");


  console.info("Client 4 connected.");
  console.info("Current users 3.");
  console.info("Client 4 disconnected.");
  console.info("Current users 2.");
  console.info("Client 2 disconnected.");
  console.info("Current users 1.");
  console.info("Client 1 disconnected.");
  console.info("Current users 0.");
  console.info("Server is empty.");

  close_server();
  quit.now(quit.Success);
}
