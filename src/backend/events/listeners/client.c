#include <events/listeners/client.h>
#include <shared/utils/encryption.h>
#include <shared/utils/console.h>
#include <server/sockets.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <shared/utils/common.h>
#include <events/events.h>

static char *create_handshake_response(const char *key) {
  var response = responses.create("HTTP/1.1 101 Switching Protocols");
  responses.add_header("Upgrade: websocket", &response);
  responses.add_header("Connection: Upgrade", &response);

  let websocket_key = encryption.websocket(key);
  let header = str("Sec-WebSocket-Accept: %s", websocket_key);
  responses.add_header(header, &response);

  free(websocket_key);
  free(header);
  return response;
}

static void on_input(struct epoll_event event) {
  static const char *const WebSocketWildcard = "Sec-WebSocket-Key: %s";
  static const char *const WebSocketKey = "Sec-WebSocket-Key:";

  response_t handshake_response = NULL;
  char *line;
  char *key = malloc(DefaultBufferSize);
  while ((line = sockets.readline(event.data.fd)) != NULL) {
    console.info("Read from socket: [%s]", line);
    if (strstr(line, WebSocketKey)) {
      sscanf(line, WebSocketWildcard, key);
      console.info("Found websocket key: [%s]", key);
      handshake_response = create_handshake_response(key);
      console.info("Created handshake protocol");
    }
  }

  responses.send(handshake_response, event.data.fd);
}
static void on_hangup(struct epoll_event event) {
  console.event("Removing socket '%d' from watch", event.data.fd);

  events.remove(event.data.fd);
  listeners.remove(event.data.fd);
  listeners.premature_exit(event.data.fd);
}
static Listener create(void) {
  return (Listener) {.on_input=on_input, .on_hangup=on_hangup, .on_error=on_hangup, .should_exit=false};
}

const struct client_listener_t client_listener = {
        .create = create
};
