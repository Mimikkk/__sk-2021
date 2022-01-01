#include <shared/utils/console.h>
#include <shared/utils/error_handle.h>
#include <server/server.h>
#include <events/chain.h>
#include <events/threads/thread.h>
#include <unistd.h>

int main(void) {
  console.info("Starting server...");
  server.open();
  console.info("Started server %s.", server.info());

  console.info("Creating epoll thread...");
  Thread thread = threads.create(chains.start);
  while (threads.is_alive(thread)) {
//    console.info("Waiting for epoll thread to finish...");
    sleep(2);
  }

  console.info("Exiting application, waiting on all threads...");
  threads.join(thread);
  console.info("Joined");

  server.close();
  quit.now(quit.Success);
}
