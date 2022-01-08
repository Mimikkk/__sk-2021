#include <events/listeners/client/client.h>
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
static void handle_messages(struct epoll_event event);
static void handle_unexpected(struct epoll_event event);

static void handle_first_message(struct epoll_event event) {
  static const char *const WebSocketWildcard = "Sec-WebSocket-Key: %s";
  static const char *const WebSocketKey = "Sec-WebSocket-Key:";
  static const char *const NameWildcard = "GET /%s ";
  static const char *const NameKey = "GET /";

  let fd = event.data.fd;
  var listener = listeners.get(fd);

  char *line;
  while ((line = sockets.readline(fd)) != NULL) {
    console.info("Read from socket: [%s]", line);
    if (strstr(line, NameKey)) {
      char *name = malloc(DefaultBufferSize);
      sscanf(line, NameWildcard, name);
      console.info("Found name: [%s]", name);

      if (listeners.contains_name(name)) {
        console.error("Contains duplicate socket identified as '%s'", name);
        free(name);
        return handle_unexpected(event);
      }
      listener->info.name = str(name);
      free(name);
    } else if (strstr(line, WebSocketKey)) {
      char *key = malloc(DefaultBufferSize);
      sscanf(line, WebSocketWildcard, key);
      console.info("Found websocket key: [%s]", key);

      listener->info.response = responses.handshake(key);
      console.event("Created handshake protocol");
      free(key);
    }
  }

  free(line);
  listener->on_input = handle_messages;
  listener->on_output = shake_hand;
}

static const char CommandInfo[] = "<>server_info<>";
static const char CommandMessage[] = "<>message<>";
static const char CommandMessageWildcard[] = "<>message<><%[^|]|%[^>]>";

const struct commands_t {
    const char *const MessageWildcard;
    const char *const Message;
    const char *const Info;
} commands = {
        .Info = CommandInfo,
        .Message = CommandMessage,
        .MessageWildcard = CommandMessageWildcard
};

static void handle_messages(struct epoll_event event) {
  console.info("just reading a datagram :)");
  let fd = event.data.fd;
  let listener = listeners.get(fd);

  let datagram = datagrams.try_read(fd);
  if (!datagram) return;
  console.info("Read: [%s] from '%s' at '%d'", datagram, listener->info.name, fd);

  if (strstr(datagram, commands.Message)) {
    char *message = malloc(DefaultBufferSize);
    char *recipient = malloc(DefaultBufferSize);
    sscanf(datagram, commands.MessageWildcard, recipient, message);
    console.event("Message from '%s' to '%s' with [%s]", listener->info.name, recipient, message);
  } else if (strstr(datagram, commands.Info)) {
    console.event("'%s' requested server info", listener->info.name);
    datagrams.write(fd, str("'%s' is your name :)", listener->info.name));
    console.event("Server send server info");
  }
}

static void shake_hand(struct epoll_event event) {
  let fd = event.data.fd;
  let listener = listeners.get(fd);

  console.event("Shook hands with '%s' at '%d'", listener->info.name, fd);
  responses.send(listener->info.response, fd);
  listener->on_output = NULL;
}

static void handle_unexpected(struct epoll_event event) {
  let fd = event.data.fd;
  console.event("Removing socket '%d' from watch", fd);

  events.remove(fd);
  listeners.clear(fd);
  listeners.premature_exit(fd);
}
static Listener create(void) {
  return (Listener) {.on_input=handle_first_message, .on_hangup=handle_unexpected, .on_error=handle_unexpected};
}

const struct client_listener_t client_listener = {
        .create = create
};
