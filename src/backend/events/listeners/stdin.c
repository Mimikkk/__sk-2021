#include <events/listeners/stdin.h>
#include <shared/utils/console.h>
#include <events/chain.h>

static void stop_chain(struct epoll_event event) {
  console.event("Input from standard input, stopping the event chain");
  chains.stop();
}

static Listener create(void) {
  return (Listener) {.on_read=stop_chain};
}

const struct stdin_listener_t stdin_listener = {
        .create = create
};
