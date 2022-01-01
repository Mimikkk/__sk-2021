#include <events/listeners/client.h>
#include <shared/utils/console.h>
#include <shared/utils/common.h>
#include <unistd.h>
#include <malloc.h>

static char *read_socket_line(int fd);
static void on_input(struct epoll_event event) {
  char*line;
  while ((line=read_socket_line(event.data.fd)) != NULL) {
    console.log("Read from socket: [%s]", line);
  }
}

static Listener create(void) {
  return (Listener) {.on_input=on_input};
}

const struct client_listener_t client_listener = {
        .create = create
};
