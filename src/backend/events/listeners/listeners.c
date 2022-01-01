#include <events/listeners/listeners.h>
#include <shared/utils/common.h>
#include <shared/utils/error_handle.h>
#include <malloc.h>

enum { MaxListeners = 1024 };

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
}

static void remove_listener(size_t index) {
  set(index, (Listener) {});
}
static void premature_exit(size_t index) {
  get(index)->should_exit = true;
}

const struct listeners_lib listeners = {
        .get = get,
        .set = set,

        .remove = remove_listener,
        .premature_exit = premature_exit,
};
