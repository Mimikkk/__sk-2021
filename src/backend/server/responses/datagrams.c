#include <server/responses/datagrams.h>
#include <shared/utils/console.h>
#include <shared/utils/common.h>
#include <server/sockets.h>
#include <malloc.h>
#include <stdint.h>
#include <string.h>

enum {
    ContinueReadOperation = 0x0,
    TextOperation = 0x1,
    BinaryOperation = 0x2,
    CloseOperation = 0x8,
    PingOperation = 0x9,
    PongOperation = 0xA,
};
enum {
    WebsocketHeaderSize = 10,
    FinishBitLayout = 0x80,
    MaskBitLayout = 0x80,
};

static unsigned char *buffer;
static void close_response(int fd) {
  if (!sockets.try_send(fd, &(unsigned char[2]) {FinishBitLayout | CloseOperation, 0x00}, 2))
    console.error("Failed to write close response");
}

static bool ping_response(int fd, uint64_t len, const unsigned char masking_key[4]) {
  buffer = calloc(8, sizeof(char));

  buffer[0] = FinishBitLayout | PongOperation;
  if (!sockets.try_send(fd, buffer, 1)) return false;

  if (len < 126) {
    buffer[0] = len & 0x7F;
    if (!sockets.try_send(fd, buffer, 1)) return false;
  } else if (len < (1 << 16)) {
    buffer[0] = 126;
    if (!sockets.try_send(fd, buffer, 1)) return false;
    buffer[0] = (len >> 8) & 0xFF;
    buffer[1] = len & 0xFF;
    if (!sockets.try_send(fd, buffer, 2)) return false;
  } else {
    buffer[0] = 127;
    if (!sockets.try_send(fd, buffer, 1)) return false;
    buffer[0] = (len >> 56) & 0xFF;
    buffer[1] = (len >> 48) & 0xFF;
    buffer[2] = (len >> 40) & 0xFF;
    buffer[3] = (len >> 32) & 0xFF;
    buffer[4] = (len >> 24) & 0xFF;
    buffer[5] = (len >> 16) & 0xFF;
    buffer[6] = (len >> 8) & 0xFF;
    buffer[7] = len & 0xFF;
    if (!sockets.try_send(fd, buffer, 8)) return false;
  }

  for (int i = 0; i < len; i++) {
    if (!sockets.try_read(fd, buffer, 1)) return false;
    buffer[0] ^= masking_key[i % 4];
    if (!sockets.try_send(fd, buffer, 1)) return false;
  }
  return true;
}

static uint64_t check_for_extra(uint64_t len) {
  switch (len) {
    case 126:
      return 2;
    case 127:
      return 8;
    default:
      return 0;
  }
}
static uint64_t find_size_with_extra(int fd, uint64_t extra) {
  if (!sockets.try_read(fd, buffer, extra)) return -1;

  var len = 0;
  for (int i = 0; i < extra; ++i) len = (len << 8) + buffer[i];
  return len;
}
static uint64_t find_size(int fd, uint64_t length) {
  let extra = check_for_extra(length);
  return extra ? find_size_with_extra(fd, extra) : length;
}
static char *read_response(int fd) {
  buffer = calloc(8, sizeof(char));

  int has_mask;
  unsigned char masking_key[4] = {0};
  unsigned char is_fin;
  unsigned char opcode;

  uint64_t len;
  uint64_t current_offset = 0;
  char *data = calloc(DefaultBufferSize, sizeof(char));

  do {
    if (!sockets.try_read(fd, buffer, 2)) return NULL;

    is_fin = buffer[0] & FinishBitLayout;
    opcode = buffer[0] & 0x0F;
    has_mask = buffer[1] & MaskBitLayout;

    if ((len = find_size(fd, buffer[1] & 0x7F)) < 0) return NULL;
    if (!has_mask) {
      close_response(fd);
      return NULL;
    }

    if (!sockets.try_read(fd, masking_key, 4)) return NULL;

    switch (opcode) {
      case CloseOperation:
        close_response(fd);
        return NULL;
      case PingOperation:
        if (!ping_response(fd, len, masking_key)) console.error("failed to send ping response");
        return NULL;
      case PongOperation:
        sockets.try_read(fd, NULL, len);
        return NULL;
      case TextOperation:
      case ContinueReadOperation:
        if (!sockets.try_read(fd, &data[current_offset], len)) return NULL;

        for (int i = 0; i < len; ++i) ((unsigned char *) data)[current_offset + i] ^= masking_key[i % 4];
        current_offset += len;
        break;
      default:
        console.error("Invalid dataframe");
        return NULL;
    }
  } while (!is_fin);

  return data;
}
static char *try_read_response(int fd) {
  let datagram = read_response(fd);
  if (datagram == NULL) console.info("Invalid datagram");
  return datagram;
}

void write_response(int fd, const char *message) {
  int websocket_hdr_len;
  int i;

  let length = strlen(message);
  unsigned char websocket_message[WebsocketHeaderSize];
  websocket_message[0] = FinishBitLayout | TextOperation;
  if (length < 126) {
    websocket_hdr_len = 2;
    websocket_message[1] = length & 0x7F;
  } else if (length < (1 << 16)) {
    websocket_hdr_len = 4;
    websocket_message[1] = 126;
    websocket_message[2] = (length >> 8) & 0xFF;
    websocket_message[3] = length & 0xFF;
  } else {
    websocket_hdr_len = 10;
    websocket_message[1] = 127;
    for (i = 0; i < 8; ++i) websocket_message[i + 2] = (length >> ((7 - i) * 8)) & 0xFF;
  }

  sockets.try_send(fd, websocket_message, websocket_hdr_len);
  sockets.try_send(fd, (void *) message, length);
}

const struct datagrams_lib datagrams = {
        .try_read = try_read_response,
        .read = read_response,
        .write = write_response,
};
