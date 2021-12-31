#include <shared/imports.h>
#include <events/events.h>
#include <shared/utils/console.h>
#include <events/threads/thread.h>
#include <events/threads/chain.h>
#include <shared/utils/error_handle.h>

enum { MaxEvents = 10 };

static size_t event_count;
static struct epoll_event listened_events[MaxEvents];

static void add_event(int fd, uint32_t events) {
  let had_error = epoll_ctl(*chains.fd, EPOLL_CTL_ADD, fd, &(struct epoll_event) {events, .data.fd=fd});
  quit.on(had_error, "Events add failure");
}
static void remove_event(int fd) {
  let had_error = epoll_ctl(*chains.fd, EPOLL_CTL_DEL, fd, NULL) < 0;
  quit.on(had_error, "Events remove failure");
}
static void await_events(void) {
  event_count = epoll_wait(*chains.fd, listened_events, MaxEvents, -1);
  let had_error = event_count < 0;
  quit.on(had_error, "Epoll wait error");
}

const struct events_lib events = {
        .add = add_event,
        .remove = remove_event,
        .await = await_events,

        .awaited_count = &event_count,
        .awaited = listened_events,
};
