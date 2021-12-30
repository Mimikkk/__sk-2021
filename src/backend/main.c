#include <shared/utils/console.h>
#include <shared/utils/error_handle.h>
#include <shared/utils/imports.h>
#include <shared/utils/common.h>
#include <server/server.h>

static const int MaxEvents = 10;

static noreturn handle_events(void) {
  struct epoll_event event;
  struct epoll_event events[MaxEvents];
  int nfds;
  int epollfd;
  int connection_socket;

  quit.on((epollfd = epoll_create1(0)) < 0, "Epoll creation failure");

  event.events = EPOLLIN;
  event.data.fd = *server.socket;
  quit.on(epoll_ctl(epollfd, EPOLL_CTL_ADD, *server.socket, &event) < 0, "Epoll control failure");

  loop {
    quit.on((nfds = epoll_wait(epollfd, events, MaxEvents, -1)) == -1, "epoll_wait failure");

    for (int n = 0; n < nfds; ++n) {
      if (events[n].data.fd == *server.socket) {
        quit.on((connection_socket = accept(*server.socket, NULL, NULL)) == -1, "accept failure");
        fcntl(connection_socket, F_SETFL, O_NONBLOCK, 1);

        event.events = EPOLLIN | EPOLLET;
        event.data.fd = connection_socket;
        quit.on(epoll_ctl(epollfd, EPOLL_CTL_ADD, connection_socket, &event) == -1,
                "epoll_ctl: socket connection failure");
        printf("server is so cool\n");
      }

      printf("socket %d\n", events[n].data.fd);
    }
  }
}

int main(void) {
  console.info("Starting server...");
  server.open();
  console.info("Started server %s.", server.info());

  console.info("Waiting for events...");
  handle_events();

  console.info("Client 1 connecting...");
  console.info("Client 1 connected.");
  console.info("Current users 1.");
  console.info("Client 2 connecting...");
  console.info("Client 2 connected.");
  console.info("Current users 2.");
  console.info("Client 3 connecting...");
  console.info("Client 3 connected.");
  console.info("Current users 3.");

  console.info("Client 4 connecting...");
  console.info("Client 4 refused ( Max users reached ).");

  console.info("Client 3 disconnected.");
  console.info("Current users 2.");


  console.info("Client 4 connected.");
  console.info("Current users 3.");
  console.info("Client 4 disconnected.");
  console.info("Current users 2.");
  console.info("Client 2 disconnected.");
  console.info("Current users 1.");
  console.info("Client 1 disconnected.");
  console.info("Current users 0.");
  console.info("Server is empty.");

  server.close();
  quit.now(quit.Success);
}
