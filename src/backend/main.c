#include <shared/utils/console.h>
#include <shared/utils/error_handle.h>
#include <events/threads/thread.h>
#include <server/server.h>
#include <events/chain.h>
#include <stdlib.h>
#include "statistics/statistics.h"

int main(int argc, char *argv[]) {
  *server.raw_address = argc > 1 ? argv[1] : "127.0.0.1";
  *server.raw_port = argc > 2 ? (int) strtol(argv[2], NULL, 10) : 8080;

  console.info("Starting server...");
  server.open();
  console.info("Started server %s.", server.info());

  console.info("Creating epoll thread...");
  Thread chain = threads.create(chains.start);
  Thread statistic = threads.create(statistics.start);

  threads.join(chain);

  console.info("Exiting application, waiting on all threads...");
  threads.kill(statistic);
  threads.join(statistic);
  console.info("Joined");

  server.close();
  quit.now(quit.Success);
}
