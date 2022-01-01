#include <shared/utils/sockets.h>
#include <shared/imports.h>

char *read_socket_line(int fd) {
  size_t buffer_size = 0;
  size_t read_buffer = 0;
  ssize_t size;
  char symbol;
  char *buffer;

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

const struct sockets_lib sockets = {
        .readline = read_socket_line
};