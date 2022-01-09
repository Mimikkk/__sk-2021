#include <events/chain.h>
#include <shared/utils/error_handle.h>
#include <server/server.h>
#include <shared/imports.h>
#include <shared/utils/console.h>
#include <events/events.h>
#include <events/listeners/listeners.h>
#include <events/listeners/stdin.h>
#include <events/listeners/server.h>

enum { InputFd = 0, };
static int epollfd;
static bool is_running = true;

static void create_epoll(void) {
  epollfd = epoll_create1(0);
  let had_error = epollfd < 0;
  quit.on(had_error, "Epoll creation failure");
}

static void epoll_initialize(void) {
  create_epoll();

  console.info("Added server socket and stdin to await_events");
  events.add(*server.socket, EPOLLIN | EPOLLET);
  events.add(InputFd, EPOLLIN | EPOLLET);
  listeners.set(InputFd, stdin_listener.create());
  listeners.set(*server.socket, server_listener.create());

  console.info("Epoll awaiting events...");
  while (is_running) {
    events.await();
    events.handle();
  }
}

void stop(void) {
  is_running = false;
  console.info("Epoll exiting");
}

const struct chain_lib chains = {
        .start = epoll_initialize,
        .fd = &epollfd,
        .stop = stop
};