#include <events/listeners/stdin.h>
#include <shared/utils/console.h>
#include <events/threads/chain.h>

void on_input(struct epoll_event event) {
  console.log("Hangup");
  chains.stop();
}

Listener create(void) {
  return (Listener) {.on_input=on_input};
}

const struct stdin_listener_t stdin_listener = {
        .create = create
};
