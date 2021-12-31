#include <events/threads/chain.h>
#include <shared/utils/error_handle.h>
#include <server/server.h>
#include <shared/imports.h>
#include <shared/utils/console.h>
#include <events/events.h>

enum { InputFd = 0, };
static int epollfd;
static bool is_running = true;

static void create_epoll(void) {
  epollfd = epoll_create1(0);
  quit.on(epollfd < 0, "Epoll creation failure");
}

static void handle_events(void) {
  for (int n = 0; n < *events.awaited_count; ++n) {
    console.info("socket '%d' event\n", events.awaited[n].data.fd);

    if (events.awaited[n].data.fd == InputFd) {
      console.log("Server Stdin");
      is_running = false;
      return;
    } else if (events.awaited[n].data.fd == *server.socket) {
      let fd = server.accept();
      events.add(fd, EPOLLIN | EPOLLET);
      console.info("Added new socket '%d'", fd);
    }
  }
}

static void epoll_initialize(void) {
  create_epoll();

  console.info("Added server socket and stdin to await_events.");
  events.add(*server.socket, EPOLLIN | EPOLLET);
  events.add(InputFd, EPOLLIN | EPOLLET);

  console.info("Epoll awaiting events...");
  while (is_running) {
    events.await();
    handle_events();
  }
  console.info("Epoll exiting.");
}

const struct chain_lib chains = {
        .start = epoll_initialize,
        .fd = &epollfd,
};