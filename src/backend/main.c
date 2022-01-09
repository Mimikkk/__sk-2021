#include <shared/utils/console.h>
#include <shared/utils/error_handle.h>
#include <events/threads/thread.h>
#include <server/server.h>
#include <events/chain.h>
#include <unistd.h>
#include <statistics/statistics.h>

int main(void) {
  console.info("Starting server...");
  server.open();
  console.info("Started server %s.", server.info());

  console.info("Creating epoll thread...");
  Thread chain = threads.create(chains.start);
  Thread statistic = threads.create(statistics.start);
  while (threads.is_alive(chain)) sleep(2);

  console.info("Exiting application, waiting on all threads...");
  threads.kill(statistic);
  
  threads.join(chain);
  threads.join(statistic);
  console.info("Joined");

  server.close();
  quit.now(quit.Success);
}
