#include <events/threads/chain.h>
#include <shared/utils/error_handle.h>
#include <server/server.h>
#include <shared/imports.h>
#include <shared/utils/console.h>
#include <events/events.h>
#include <events/listeners/listeners.h>
#include <events/listeners/stdin.h>

enum { InputFd = 0, };
static int epollfd;
static bool is_running = true;

static void create_epoll(void) {
  epollfd = epoll_create1(0);
  let had_error = epollfd < 0;
  quit.on(had_error, "Epoll creation failure");
}

static void handle_events(void) {
  for (int n = 0; n < *events.awaited_count; ++n) {
    let event = events.awaited[n];
    console.log(events.info(event));
    events.handle(event);
  }
}

static void epoll_initialize(void) {
  create_epoll();

  console.info("Added server socket and stdin to await_events.");
  events.add(*server.socket, EPOLLIN | EPOLLET);
  events.add(InputFd, EPOLLIN | EPOLLET);
  listeners.set(InputFd, stdin_listener.create());

  console.info("Epoll awaiting events...");
  while (is_running) {
    events.await();
    handle_events();
  }
}

void stop(void) {
  is_running = false;
  console.info("Epoll exiting.");
}

const struct chain_lib chains = {
        .start = epoll_initialize,
        .fd = &epollfd,
        .stop = stop
};