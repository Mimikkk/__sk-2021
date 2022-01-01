#include <events/listeners/client.h>
#include <shared/utils/encryption.h>
#include <shared/utils/console.h>
#include <server/sockets.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <shared/utils/common.h>
#include <events/events.h>

static response_t current_response = NULL;

static void shake_hand(struct epoll_event event);
static void read_hand(struct epoll_event event);
static void just_read(struct epoll_event event);

static void read_hand(struct epoll_event event) {
  static const char *const WebSocketWildcard = "Sec-WebSocket-Key: %s";
  static const char *const WebSocketKey = "Sec-WebSocket-Key:";

  char *line;
  char *key = malloc(DefaultBufferSize);
  while ((line = sockets.readline(event.data.fd)) != NULL) {
    console.info("Read from socket: [%s]", line);
    if (strstr(line, WebSocketKey)) {
      sscanf(line, WebSocketWildcard, key);
      console.info("Found websocket key: [%s]", key);
      current_response = responses.handshake(key);
      console.event("Created handshake protocol");
    }
  }

  free(key);
  free(line);
  listeners.get(event.data.fd)->on_input = just_read;
  listeners.get(event.data.fd)->on_output = shake_hand;
}
static void just_read(struct epoll_event event) {
  console.info("just reading :)");

  char *line;
  while ((line = sockets.readline(event.data.fd)) != NULL) {
    console.info("Read from socket: [%s]", line);
  }
}
static void shake_hand(struct epoll_event event) {
  console.event("Shook hands with '%d'", event.data.fd);

  responses.send(current_response, event.data.fd);

  var listener = listeners.get(event.data.fd);
  listener->info.shook_hands = true; 
  listener->on_output = NULL;
}

static void handle_unexpected(struct epoll_event event) {
  console.event("Removing socket '%d' from watch", event.data.fd);

  events.remove(event.data.fd);
  listeners.remove(event.data.fd);
  listeners.premature_exit(event.data.fd);
}
static Listener create(void) {
  return (Listener) {.on_input=read_hand, .on_hangup=handle_unexpected, .on_error=handle_unexpected};
}

const struct client_listener_t client_listener = {
        .create = create
};
