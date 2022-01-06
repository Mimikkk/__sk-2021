#include <events/listeners/client.h>
#include <shared/utils/encryption.h>
#include <shared/utils/console.h>
#include <server/sockets.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <shared/utils/common.h>
#include <events/events.h>
#include <unistd.h>
#include <sys/socket.h>
#include <server/responses/datagrams.h>

static void shake_hand(struct epoll_event event);
static void read_hand(struct epoll_event event);
static void just_read(struct epoll_event event);
static void handle_unexpected(struct epoll_event event);

static void read_hand(struct epoll_event event) {
  static const char *const WebSocketWildcard = "Sec-WebSocket-Key: %s";
  static const char *const WebSocketKey = "Sec-WebSocket-Key:";
  static const char *const NameWildcard = "GET /%s ";
  static const char *const NameKey = "GET /";

  let fd = event.data.fd;
  var listener = listeners.get(fd);

  char *line;
  char *buffer = malloc(DefaultBufferSize);
  while ((line = sockets.readline(fd)) != NULL) {
    console.info("Read from socket: [%s]", line);
    if (strstr(line, NameKey)) {
      sscanf(line, NameWildcard, buffer);
      console.info("Found name: [%s]", buffer);
      if (listeners.contains_name(buffer)) {
        console.error("Contains duplicate socket identified as '%s'", buffer);
        free(buffer);
        return handle_unexpected(event);
      }
      listener->info.name = str(buffer);
    } else if (strstr(line, WebSocketKey)) {
      sscanf(line, WebSocketWildcard, buffer);
      console.info("Found websocket key: [%s]", buffer);

      listener->info.response = responses.handshake(buffer);
      console.event("Created handshake protocol");
    }
  }

  free(buffer);
  free(line);
  listener->on_input = just_read;
  listener->on_output = shake_hand;
}
static void just_read(struct epoll_event event) {
  console.info("just reading a datagram :)");
  let fd = event.data.fd;
  let datagram = datagrams.read(fd);
  console.log("frame data: [%s]", datagram);
}

static void shake_hand(struct epoll_event event) {
  let fd = event.data.fd;
  var listener = listeners.get(fd);

  console.event("Shook hands with '%s' at '%d'", listener->info.name, fd);
  responses.send(listener->info.response, fd);
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
