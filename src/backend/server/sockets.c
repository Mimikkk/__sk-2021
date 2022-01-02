#include <server/sockets.h>
#include <shared/imports.h>
#include <events/listeners/listeners.h>
#include <events/chain.h>
#include "shared/utils/console.h"

static char *read_socket_line(int fd) {
  size_t buffer_size = 0;
  size_t read_buffer_size = 0;
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
    if (!buffer_size || read_buffer_size == buffer_size) buffer = realloc(buffer, buffer_size += 128);
    buffer[read_buffer_size++] = symbol;
  }

  if (read_buffer_size > 0 && buffer[read_buffer_size - 1] == '\r') --read_buffer_size;
  if (!buffer_size || read_buffer_size == buffer_size) buffer = realloc(buffer, buffer_size + 1);
  buffer[read_buffer_size] = '\0';
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
        .read = read,
};