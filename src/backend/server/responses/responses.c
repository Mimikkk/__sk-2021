#include <server/responses/responses.h>
#include <shared/utils/common.h>
#include <server/sockets.h>
#include <malloc.h>
#include <shared/utils/encryption.h>

static response_t create(const char *method) {
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

static response_t handshake(const char *key) {
  var response = create("HTTP/1.1 101 Switching Protocols");
  add_header("Upgrade: websocket", &response);
  add_header("Connection: Upgrade", &response);

  let websocket_key = encryption.websocket(key);
  let header = str("Sec-WebSocket-Accept: %s", websocket_key);
  add_header(header, &response);

  free(websocket_key);
  free(header);
  return response;
}

const struct responses_lib responses = {
        .create = create,
        .handshake = handshake,
        .send = send_response,
};
