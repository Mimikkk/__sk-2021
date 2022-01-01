#include <events/listeners/client.h>
#include <shared/utils/encryption.h>
#include <shared/utils/console.h>
#include <shared/utils/sockets.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <shared/utils/common.h>
#include <sys/socket.h>

static char *const response1 = "HTTP/1.1 101 Switching Protocols\r\n" \
"Upgrade: websocket\r\n" \
"Connection: Upgrade\r\n" \
"Sec-WebSocket-Accept: ";
static char *const response2 = "\r\n\r\n";

static char *create_handshake_response(char *key) {
  char *response = malloc(strlen(response1) + strlen(response2) + strlen(key) + 1);
  response[0] = '\0';

  strcat(response, response1);
  strcat(response, key);
  strcat(response, response2);
  return response;
}

static void on_input(struct epoll_event event) {
  static const char *const WebSocketWildcard = "Sec-WebSocket-Key: %s";
  static const char *const WebSocketKey = "Sec-WebSocket-Key:";

  char *line;
  char *key = malloc(1024);
  while ((line = sockets.readline(event.data.fd)) != NULL) {
    console.log("Read from socket: [%s]", line);
    if (strstr(line, WebSocketKey) && sscanf(line, WebSocketWildcard, key)) {

      console.log("Found websocket key: [%s]", str(key));
      console.log("Expects to receive: [%s]", create_handshake_response(encryption.websocket(key)));
      let x = create_handshake_response(encryption.websocket(key));
      send(event.data.fd, x, strlen(x), 0);
    }
  }
  
}


static Listener create(void) {
  return (Listener) {.on_input=on_input};
}

const struct client_listener_t client_listener = {
        .create = create
};
