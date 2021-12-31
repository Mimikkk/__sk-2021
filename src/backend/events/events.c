#include <shared/utils/error_handle.h>
#include <shared/imports.h>
#include <server/server.h>
#include <events/events.h>
#include <shared/utils/console.h>
#include <events/threads/thread.h>

enum {
    InputFd = 0,
    MaxEvents = 10
};

static struct epoll_event events[MaxEvents];
static int nfds;
static int epollfd;
static int connection_socket;

static int watch(int fd, uint32_t events) {
  struct epoll_event event = {events, .data.fd=fd};
  return epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
}
static int stop_watching(int fd) {
  return epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
}
static int create_epoll() {
  return epollfd = epoll_create1(0);
}

static void *epoll_start() {
  quit.on(create_epoll() < 0, "Epoll creation failure");

  quit.on(watch(*server.socket, EPOLLIN | EPOLLET) < 0, "Control config failure.");
  quit.on(watch(InputFd, EPOLLIN | EPOLLET) < 0, "Control config failure.");

  console.info("Epoll waiting for events...");
  loop {
    quit.on((nfds = epoll_wait(epollfd, events, MaxEvents, -1)) < 0, "Wait failure.");

    for (int n = 0; n < nfds; ++n) {
      printf("socket %d\n", events[n].data.fd);
      if (events[n].data.fd == InputFd) {
        console.log("Server Stdin");
        return NULL;
      }
      if (events[n].data.fd == *server.socket) {
        quit.on((server.accept()) < 0, "accept failure");

        quit.on(watch(connection_socket, EPOLLIN | EPOLLET) < 0, "epoll_ctl: socket connection failure");
        printf("server is so cool\n");
      }
    }
  }
}

static void handle_events(void) {
  console.info("Creating epoll thread...");

  Thread thread = threads.create(epoll_start);
  while (threads.is_alive(thread)) {
    console.info("Waiting for epoll thread to finish...");
    sleep(2);
  }

  console.info("Exiting application, waiting on all threads...");
  threads.join(thread);
  console.info("Joined");
}

const struct events_lib event_chain = {
        .watch = handle_events,
};
