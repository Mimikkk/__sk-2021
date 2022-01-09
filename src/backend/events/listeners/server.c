#include <events/listeners/server.h>
#include <shared/utils/console.h>
#include <shared/utils/common.h>
#include <server/server.h>
#include <events/events.h>
#include <statistics/statistics.h>
#include "events/listeners/client/client.h"

static void add_new_connection(struct epoll_event event) {
  let fd = server.accept();
  events.add(fd, EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLET);
  listeners.set(fd, client_listener.create());
  console.info("Added new client socket '%d'", fd);
}

static void handle_input(struct epoll_event event) {
  add_new_connection(event);
}

static Listener create(void) {
  return (Listener) {.on_read=handle_input};
}

const struct server_listener_t server_listener = {
        .create = create
};
