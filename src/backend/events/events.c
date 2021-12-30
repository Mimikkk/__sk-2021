#include <shared/utils/error_handle.h>
#include <shared/utils/imports.h>
#include <server/server.h>
#include <events/events.h>

enum { MaxEvents = 10 };

static struct epoll_event events[MaxEvents];
static int nfds;
static int epollfd;
static int connection_socket;

static struct epoll_event event;
static int watch(int fd) {
  return epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
}
static int stop_watching(int fd) {
  return epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &event);
}

static noreturn handle_events(void) {
  quit.on((epollfd = epoll_create1(0)) < 0, "Epoll creation failure");

  event.events = EPOLLIN;
  event.data.fd = *server.socket;
  quit.on(watch(*server.socket) < 0, "Control config failure.");

  loop {
    quit.on((nfds = epoll_wait(epollfd, events, MaxEvents, -1)) < 0, "Wait failure.");

    for (int n = 0; n < nfds; ++n) {
      if (events[n].data.fd == *server.socket) {
        quit.on((connection_socket = accept(*server.socket, NULL, NULL)) < 0, "accept failure");
        fcntl(connection_socket, F_SETFL, O_NONBLOCK, 1);

        event.events = EPOLLIN | EPOLLET;
        event.data.fd = connection_socket;
        quit.on(watch(connection_socket) < 0, "epoll_ctl: socket connection failure");
        printf("server is so cool\n");
      }

      printf("socket %d\n", events[n].data.fd);
    }
  }
}

const struct events_lib event_chain = {
        .watch = handle_events,
};
