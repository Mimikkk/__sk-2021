#include <events/listeners/listeners.h>
#include <shared/utils/common.h>
#include <shared/utils/error_handle.h>
#include <string.h>

enum { MaxListeners = 1024 };

static Listener fds[MaxListeners];

static ListenerInfo empty_info(int fd) {
  return (ListenerInfo) {.fd=fd, .should_exit=false};
}
static Listener empty(void) {
  return (Listener) {};
}

static Listener *get(int fd) {
  quit.on(fd < 0 || fd >= MaxListeners, "Index out of range");

  var listener = &fds[fd];
  listener->info.should_exit = false;
  return listener;
}
static void set(int fd, const Listener listener) {
  quit.on(fd < 0 || fd >= MaxListeners, "Index out of range");

  fds[fd] = listener;
  fds[fd].info = empty_info(fd);
}
static void clear(int fd) {
  set(fd, empty());
}

static void premature_exit(int fd) {
  get(fd)->info.should_exit = true;
}

static bool name_exists(int fd) {
  return fds[fd].info.name != NULL && strcmp(fds[fd].info.name, "server") != 0;
}
static bool contains_name(const char *name) {
  for (int fd = 0; fd < MaxListeners; ++fd)
    if (name_exists(fd) && strcmp(fds[fd].info.name, name) == 0)
      return true;
  return false;
}

static char *names_joined(void) {
  var names = "";
  for (int fd = 0; fd < MaxListeners; ++fd)
    if (name_exists(fd))
      names = strcmp(names, "") == 0
              ? str("\"%s\"", fds[fd].info.name)
              : str("%s, \"%s\"", names, fds[fd].info.name);
  names = str("[%s]", names);
  return names;
}

static Listener *get_by_name(const char *name) {
  for (int fd = 0; fd < MaxListeners; ++fd)
    if (name_exists(fd) && strcmp(fds[fd].info.name, name) == 0)
      return &fds[fd];
  return NULL;
}

const struct listeners_lib listeners = {
        .get = get,
        .get_by_name = get_by_name,
        .set = set,
        .empty = empty,
        .clear = clear,

        .premature_exit = premature_exit,
        .contains_name = contains_name,
        .names_joined = names_joined,
};
