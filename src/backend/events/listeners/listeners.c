#include <events/listeners/listeners.h>
#include <shared/utils/common.h>
#include <shared/utils/error_handle.h>
#include <malloc.h>
#include <string.h>

enum { MaxListeners = 1024 };

static Listener fds[MaxListeners];

static ListenerInfo create_listener_info(size_t fd) {
  return (ListenerInfo) {.shook_hands =false, .fd=fd};
}

static Listener *get(size_t index) {
  quit.on(index < 0 || index >= MaxListeners, "Index out of range");

  var listener = &fds[index];
  listener->should_exit = false;
  return listener;
}
static void set(size_t index, const Listener listener) {
  quit.on(index < 0 || index >= MaxListeners, "Index out of range");

  fds[index] = listener;
  fds[index].info = create_listener_info(index);
}

static void remove_listener(size_t index) {
  set(index, (Listener) {});
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

        .remove = remove_listener,
        .premature_exit = premature_exit,
        .contains_name = contains_name,
};
