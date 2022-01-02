#include <server/responses/datagrams.h>
#include <shared/utils/console.h>
#include <shared/utils/common.h>
#include <server/sockets.h>
#include <malloc.h>
#include <stdint.h>

enum {
    ContinueReadOperation = 0x0,
    TextReadOperation = 0x1,
    CloseOperation = 0x8,
    PingOperation = 0x9,
    PongOperation = 0xA,
};
enum {
    FinishBitLayout = 0x80,
    MaskBitLayout = 0x80,
};

static unsigned char *buffer;
static void close_response(int fd) {
  if (!sockets.try_write(fd, &(unsigned char[2]) {FinishBitLayout | CloseOperation, 0x00}, 2))
    console.log("Failed to write close response");
}

static bool ping_response(int fd, uint64_t len, const unsigned char masking_key[4]) {
  buffer = calloc(8, sizeof(char));

  buffer[0] = FinishBitLayout | PongOperation;
  if (!sockets.try_write(fd, buffer, 1)) return false;

  if (len < 126) {
    buffer[0] = len & 0x7F;
    if (!sockets.try_write(fd, buffer, 1)) return false;
  } else if (len < (1 << 16)) {
    buffer[0] = 126;
    if (!sockets.try_write(fd, buffer, 1)) return false;
    buffer[0] = (len >> 8) & 0xFF;
    buffer[1] = len & 0xFF;
    if (!sockets.try_write(fd, buffer, 2)) return false;
  } else {
    buffer[0] = 127;
    if (!sockets.try_write(fd, buffer, 1)) return false;
    buffer[0] = (len >> 56) & 0xFF;
    buffer[1] = (len >> 48) & 0xFF;
    buffer[2] = (len >> 40) & 0xFF;
    buffer[3] = (len >> 32) & 0xFF;
    buffer[4] = (len >> 24) & 0xFF;
    buffer[5] = (len >> 16) & 0xFF;
    buffer[6] = (len >> 8) & 0xFF;
    buffer[7] = len & 0xFF;
    if (!sockets.try_write(fd, buffer, 8)) return false;
  }

  for (size_t i = 0; i < len; i++) {
    if (!sockets.try_read(fd, buffer, 1)) return false;
    buffer[0] ^= masking_key[i % 4];
    if (!sockets.try_write(fd, buffer, 1)) return false;
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
  if (!sockets.try_read(fd, buffer, extra)) {
    console.error("failed to read extra len size");
    return -1;
  }

  var len = 0;
  for (size_t i = 0; i < extra; ++i) len = (len << 8) + buffer[i];
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
    console.log("dupa");
    if (!sockets.try_read(fd, buffer, 2)) {
      console.error("failed to read frame header");
      return NULL;
    }

    is_fin = buffer[0] & FinishBitLayout;
    opcode = buffer[0] & 0x0F;
    has_mask = buffer[1] & MaskBitLayout;

    if ((len = find_size(fd, buffer[1] & 0x7F)) < 0) return NULL;
    if (!has_mask) {
      close_response(fd);
      return NULL;
    }

    if (!sockets.try_read(fd, masking_key, 4)) {
      console.error("failed to read mask bytes");
      return NULL;
    }

    switch (opcode) {
      case CloseOperation:
        close_response(fd);
        return NULL;
      case PingOperation:
        if (!ping_response(fd, len, masking_key)) console.error("failed to send ping response");
        return NULL;
      case PongOperation:
        if (!sockets.try_read(fd, NULL, len)) console.error("failed to read len byte");
        return NULL;
      case TextReadOperation:
      case ContinueReadOperation:
        if (!sockets.try_read(fd, &data[current_offset], len)) {
          console.log("failed to read %d bytes", len);
          return NULL;
        }

        for (size_t i = 0; i < len; ++i) ((unsigned char *) data)[current_offset + i] ^= masking_key[i % 4];
        current_offset += len;
        break;
      default:
        console.error("Invalid dataframe");
        return NULL;
    }
  } while (!is_fin);

  return data;
}

const struct datagrams_lib datagrams = {
        .read = read_response,
};
