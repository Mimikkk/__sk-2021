#include <events/listeners/server.h>
#include <shared/utils/console.h>
#include <shared/utils/common.h>
#include <server/server.h>
#include <events/events.h>
#include "client.h"

static void add_new_connection(struct epoll_event event);

static void handle_input(struct epoll_event event) {
  if (listeners.get(event.data.fd)->info.shook_hands) {
    console.event("I know '%d' exists, dobry ziomeczek :)", event.data.fd);
    return;
  }

  add_new_connection(event);
}

static void add_new_connection(struct epoll_event event) {
  let fd = server.accept();
  events.add(fd, EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLET);
  listeners.set(fd, client_listener.create());
  console.info("Added new client socket '%d'", fd);
}

static Listener create(void) {
  return (Listener) {.on_input=handle_input};
}

const struct server_listener_t server_listener = {
        .create = create
};
