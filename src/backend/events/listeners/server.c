#include <events/listeners/server.h>
#include <shared/utils/console.h>
#include <shared/utils/common.h>
#include <server/server.h>
#include <events/events.h>

void on_input(struct epoll_event event) {
  let fd = server.accept();
  events.add(fd, EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLET);
  console.info("Added new client socket '%d'", fd);
}

Listener create(void) {
  return (Listener) {.on_input=on_input};
}

const struct server_listener_t server_listener = {
        .create = create
};
