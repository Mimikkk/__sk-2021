#include <events/threads/thread.h>
#include <shared/imports.h>

static uint64_t id = 0;

static Thread create_thread(void *routine) {
  Thread thread = {id++};
  pthread_create(&thread.system_id, NULL, routine, &thread);
  return thread;
}

static void kill_thread(const Thread self) {
  pthread_kill(self.system_id, SIGKILL);
}

static bool is_alive(const Thread self) {
  return pthread_kill(self.system_id, 0) == 0;
}

static void join_thread(const Thread self) {
  pthread_join(self.system_id, NULL);
}

const struct thread_lib threads = {
        .create = create_thread,
        .kill = kill_thread,
        .is_alive = is_alive,
        .join = join_thread,
};