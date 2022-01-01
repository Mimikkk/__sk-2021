#include <events/listeners/server.h>
#include <shared/utils/console.h>
#include <shared/utils/common.h>
#include <server/server.h>
#include <events/events.h>
#include "client.h"

static void on_input(struct epoll_event event) {
  let fd = server.accept();
  events.add(fd, EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLET);
  listeners.set(fd, client_listener.create());
  console.info("Added new client socket '%d'", fd);
}

static Listener create(void) {
  return (Listener) {.on_input=on_input};
}

const struct server_listener_t server_listener = {
        .create = create
};
