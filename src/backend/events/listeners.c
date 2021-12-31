#include <events/listeners.h>
#include <shared/utils/common.h>
#include <shared/utils/error_handle.h>

enum { MaxListeners = 1024 };
static Listener fds[MaxListeners];

Listener * get(size_t index) {
  quit.on(index < 0 || index >= MaxListeners, "Index out of range");

  var listener = &fds[index];
  listener->should_exit = false;
  return listener;
}

const struct listeners_lib listeners = {
        .get = get
};
