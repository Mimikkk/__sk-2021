#include <shared/utils/console.h>
#include <shared/utils/error_handle.h>
#include <events/events.h>
#include <server/server.h>

int main(void) {
  console.info("Starting server...");
  server.open();
  console.info("Started server %s.", server.info());

  console.info("Watching events...");
  event_chain.watch();
  
  server.close();
  quit.now(quit.Success);
}
