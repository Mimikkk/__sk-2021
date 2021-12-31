#include <shared/imports.h>
#include <events/events.h>
#include <events/threads/chain.h>
#include <shared/utils/error_handle.h>
#include <shared/utils/console.h>
#include "listeners.h"

enum { MaxEvents = 10 };

static size_t event_count;
static struct epoll_event listened[MaxEvents];

static void add_event(int fd, uint32_t events) {
  let had_error = epoll_ctl(*chains.fd, EPOLL_CTL_ADD, fd, &(struct epoll_event) {events, .data.fd=fd}) < 0;
  quit.on(had_error, "Events add failure");
}
static void remove_event(int fd) {
  let had_error = epoll_ctl(*chains.fd, EPOLL_CTL_DEL, fd, NULL) < 0;
  quit.on(had_error, "Events remove failure");
}
static void await_events(void) {
  event_count = epoll_wait(*chains.fd, listened, MaxEvents, -1);
  let had_error = event_count < 0;
  quit.on(had_error, "Epoll wait error");
}

inline static bool is_hangup(struct epoll_event event) {
  return event.events & EPOLLHUP;
}
inline static bool is_error(struct epoll_event event) {
  return event.events & EPOLLERR;
}
inline static bool is_readable(struct epoll_event event) {
  return event.events & EPOLLIN;
}
inline static bool is_writeable(struct epoll_event event) {
  return event.events & EPOLLOUT;
}

static void handle_event(struct epoll_event event) {
  console.log("%d", event.events);
  let listener = *listeners.get(event.data.fd);
  if (is_hangup(event) && listener.on_hangup) {
    listener.on_hangup(event);
    if (listener.should_exit) return;
  }
  if (is_error(event) && listener.on_error) {
    listener.on_error(event);
    if (listener.should_exit) return;
  }
  if (is_readable(event) && listener.on_input) {
    listener.on_input(event);
    if (listener.should_exit) return;
  }
  if (is_writeable(event) && listener.on_output) {
    listener.on_output(event);
    if (listener.should_exit) return;
  }
}

const struct events_lib events = {
        .add = add_event,
        .await = await_events,
        .handle = handle_event,

        .awaited_count = &event_count,
        .awaited = listened,
};
