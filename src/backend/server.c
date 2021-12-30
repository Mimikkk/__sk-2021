#include <shared/utils/console.h>

int main(void) {
  console.info("Starting server...");
  console.info("Started server.");

  console.log("Waiting for clients...");
  console.log("Client 1 connected.");
  console.log("Client 2 connected.");
  console.log("Client 3 connected.");
  console.log("Client 3 disconnected.");

  console.log("Client 1-2 communication message.");
  console.log("Client 1-2 communication message.");
  console.log("Client 2-1 communication message.");
  console.log("Client 1-2 communication message.");
  console.log("Client 1-2 communication closed.");
  console.log("Client 2-1 communication message.");

  console.log("Client 4 connected.");
}
