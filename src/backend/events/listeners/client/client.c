#include <events/listeners/client/client.h>
#include <shared/utils/console.h>
#include <server/sockets.h>
#include <stdio.h>
#include <malloc.h>
#include <shared/utils/common.h>
#include <events/events.h>
#include <server/responses/datagrams.h>
#include "statistics/statistics.h"

static void shake_hand(struct epoll_event event);
static void handle_messages(struct epoll_event event);
static void handle_close(struct epoll_event event);
static void send_datagram(struct epoll_event event);

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

    if (starts_with(NameKey, line)) {
      char *name = malloc(DefaultBufferSize);
      sscanf(line, NameWildcard, name);
      console.info("Found name: [%s]", name);

      if (listeners.contains_name(name)) {
        console.error("Contains duplicate socket identified as '%s'", name);
        free(name);
        return handle_close(event);
      }
      listener->info.name = str(name);
      ++(*statistics.total_connections);
      ++(*statistics.current_connections);
      free(name);
    } else if (starts_with(WebSocketKey, line)) {
      char *key = malloc(DefaultBufferSize);
      sscanf(line, WebSocketWildcard, key);
      console.info("Found websocket key: [%s]", key);

      listener->info.response = responses.handshake(key);
      console.event("Created handshake protocol");
      free(key);
    }
  }

  free(line);
  listener->on_read = handle_messages;
  listener->on_write = shake_hand;
}

static const char CommandInfo[] = "<>server_info<>";
static const char CommandClose[] = "<>close_connection<>";
static const char CommandMessage[] = "<>message<>";
static const char CommandMessageWildcard[] = "<>message<><%[^|]|%[^>]>";

const struct commands_t {
    const char *const MessageWildcard;
    const char *const Message;
    const char *const Close;
    const char *const Info;
} commands = {
        .Info = CommandInfo,
        .Close = CommandClose,
        .Message = CommandMessage,
        .MessageWildcard = CommandMessageWildcard
};

static char *Message;
static void handle_messages(struct epoll_event event) {
  console.info("just reading a datagram :)");
  let fd = event.data.fd;
  let listener = listeners.get(fd);

  let datagram = datagrams.try_read(fd);
  if (!datagram) return;
  console.info("Read: [%s] from '%s' at '%d'", datagram, listener->info.name, fd);

  if (starts_with(commands.Message, datagram)) {

    char *message = malloc(DefaultBufferSize);
    char *recipient = malloc(DefaultBufferSize);
    sscanf(datagram, commands.MessageWildcard, recipient, message);
    console.event("Message to '%s' with [%s]", listener->info.name, recipient, message);

    if (listeners.contains_name(recipient)) {
      let json = str("{ \"messenger\": \"%s\", \"recipient\": \"%s\", \"contents\": \"%s\", \"type\": \"message\" }",
                     listener->info.name, recipient, message);

      datagrams.write(listeners.get_by_name(recipient)->info.fd, json);
      free(json);
    }

    free(message);
    free(recipient);
  } else if (starts_with(commands.Info, datagram)) {
    console.event("'%s' requested server info", listener->info.name);
    let names = listeners.names_joined();
    let json = str("{ \"names\": %s, \"type\": \"info\" }", names);
    free(names);

    Message = json;
    listener->on_write = send_datagram;
    console.event("Server send server info");
  } else if (starts_with(commands.Close, datagram)) {
    handle_close(event);
    console.event("Client closes connection");
  }
}

static void shake_hand(struct epoll_event event) {
  let fd = event.data.fd;
  let listener = listeners.get(fd);

  console.event("Shook hands with '%s' at '%d'", listener->info.name, fd);
  responses.send(listener->info.response, fd);
  listener->on_write = NULL;
}

static void send_datagram(struct epoll_event event) {
  let fd = event.data.fd;
  let listener = listeners.get(fd);

  datagrams.write(fd, Message);
  free(Message);
  listener->on_write = NULL;
}

static void handle_close(struct epoll_event event) {
  let fd = event.data.fd;
  console.event("Removing socket '%d' from watch", fd);
  events.remove(fd);
  listeners.clear(fd);
  listeners.premature_exit(fd);
  --(*statistics.current_connections);
}
static Listener create(void) {
  return (Listener) {.on_read=handle_first_message, .on_hangup=handle_close, .on_error=handle_close};
}

const struct client_listener_t client_listener = {
        .create = create
};
