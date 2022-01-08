#include <events/listeners/listeners.h>
#include <shared/utils/common.h>
#include <shared/utils/error_handle.h>
#include <malloc.h>
#include <string.h>

enum { MaxListeners = 1024 };

static ListenerInfo empty_info() {
  return (ListenerInfo) {};
}
static Listener empty(void) {
  return (Listener) {};
}

static Listener fds[MaxListeners];

static Listener *get(size_t index) {
  quit.on(index < 0 || index >= MaxListeners, "Index out of range");

  var listener = &fds[index];
  listener->should_exit = false;
  return listener;
}
static void set(size_t index, const Listener listener) {
  quit.on(index < 0 || index >= MaxListeners, "Index out of range");

  fds[index] = listener;
  fds[index].info = empty_info();
}
static void clear(size_t fd) {
  set(fd, empty());
}

static void premature_exit(size_t index) {
  get(index)->should_exit = true;
}
static bool contains_name(const char *name) {
  for (size_t i = 0; i < MaxListeners; ++i) if (fds[i].info.name && strcmp(fds[i].info.name, name) == 0) return true;
  return false;
}

const struct listeners_lib listeners = {
        .get = get,
        .set = set,
        .empty = empty,
        .clear = clear,

        .premature_exit = premature_exit,
        .contains_name = contains_name,
};
