#include <server/responses/responses.h>
#include <shared/utils/common.h>
#include <server/sockets.h>
#include <malloc.h>

static response_t create_response(const char *method) {
  return str("%s\r\n", method);
}

static void add_header(const char *header, response_t *response) {
  *response = str("%s%s\r\n", *response, header);
}

static void send_response(response_t response, int fd) {
  response = str("%s\r\n", response);
  sockets.send(fd, response);
  free(response);
}

const struct responses_lib responses = {
        .create = create_response,
        .add_header = add_header,
        .send = send_response,
};
