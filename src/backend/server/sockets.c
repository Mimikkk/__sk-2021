#include <server/sockets.h>
#include <shared/imports.h>
#include <events/listeners/listeners.h>
#include <events/chain.h>
#include "shared/utils/console.h"

static char *read_socket_line(int fd) {
  size_t buffer_size = 0;
  size_t read_buffer = 0;
  ssize_t size;
  char symbol;
  char *buffer = NULL;

  while (true) {
    size = read(fd, &symbol, 1);
    if (size < 1) {
      free(buffer);
      return NULL;
    }

    if (symbol == '\n') break;

    // is more memory needed?
    if (!buffer_size || read_buffer == buffer_size) buffer = realloc(buffer, buffer_size += 128);
    buffer[read_buffer++] = symbol;
  }

  // if the line was terminated by "\r\n", ignore the
  // "\r". the "\n" is not in the buffer
  if (read_buffer > 0 && buffer[read_buffer - 1] == '\r') --read_buffer;

  // is more memory needed?
  if (!buffer_size || read_buffer == buffer_size) buffer = realloc(buffer, ++buffer_size);
  buffer[read_buffer] = '\0';
  return buffer;
}

static void send_response(int fd, const char *response) {
  send(fd, response, strlen(response), 0);
}
static void close_socket(int fd) {
  close(fd);
  listeners.set(fd, (Listener) {});
}

const struct sockets_lib sockets = {
        .close=close_socket,
        .readline = read_socket_line,
        .send = send_response,
};