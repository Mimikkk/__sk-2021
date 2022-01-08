#include <shared/imports.h>
#include <events/events.h>
#include <events/chain.h>
#include <shared/utils/error_handle.h>
#include <shared/utils/console.h>
#include <ctype.h>
#include "events/listeners/listeners.h"

enum { MaxEvents = 10 };

static size_t awaited_count;
static struct epoll_event awaited[MaxEvents];

static char *event_info_callbacks(struct epoll_event event);
static char *event_info(struct epoll_event event);

static void add_event(int fd, uint32_t events) {
  let had_error = epoll_ctl(*chains.fd, EPOLL_CTL_ADD, fd, &(struct epoll_event) {events, .data.fd=fd}) < 0;
  quit.on(had_error, "Events add failure");
}
static void remove_event(int fd) {
  let had_error = epoll_ctl(*chains.fd, EPOLL_CTL_DEL, fd, NULL) < 0;
  quit.on(had_error, "Events remove failure");
}

static void await_events(void) {
  awaited_count = epoll_wait(*chains.fd, awaited, MaxEvents, -1);
  let had_error = awaited_count < 0;
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
  console.event(event_info(event));

  let listener = listeners.get(event.data.fd);
  if (is_hangup(event) && listener->on_hangup) {
    console.info("Handling hangup event");
    listener->on_hangup(event);
    if (listener->should_exit) return;
  }
  if (is_error(event) && listener->on_error) {
    console.info("Handling error event");
    listener->on_error(event);
    if (listener->should_exit) return;
  }
  if (is_readable(event) && listener->on_input) {
    console.info("Handling read event");
    listener->on_input(event);
    if (listener->should_exit) return;
  }
  if (is_writeable(event) && listener->on_output) {
    console.info("Handling write event");
    listener->on_output(event);
    if (listener->should_exit) return;
  }
}
static void handle_events(void) {
  console.event("Awaited '%d' event/s", awaited_count);

  for (size_t n = 0; n < awaited_count; ++n) handle_event(awaited[n]);
}

const struct events_lib events = {
        .add = add_event,
        .remove = remove_event,

        .info = event_info,
        .await = await_events,
        .handle = handle_events,
};

static char *event_info_callbacks(struct epoll_event event) {
  var info = "";
  if (is_hangup(event)) info = str("%s%s", info, "hangup ");
  if (is_error(event)) info = str("%s%s", info, "error ");
  if (is_readable(event)) info = str("%s%s", info, "read ");
  if (is_writeable(event)) info = str("%s%s", info, "write ");
  return strip(info);
}
static char *event_info(struct epoll_event event) {
  let callback_types = event_info_callbacks(event);
  let result = str("Socket '%d' ready with [%d| %s] event/s", event.data.fd, event.events, callback_types);
  free(callback_types);
  return result;
}
