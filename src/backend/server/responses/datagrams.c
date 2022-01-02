#include <server/responses/datagrams.h>
#include <shared/utils/common.h>
#include <server/sockets.h>
#include <malloc.h>
#include <shared/utils/encryption.h>
#include <stdint.h>
#include <unistd.h>
#include <shared/utils/console.h>

#define OpCodeContinue  0x0
#define OpCodeText  0x1
#define OpCodeBinary  0x2
#define OpCodeClose  0x8
#define OpCodePing  0x9
#define OpCodePong  0xA
#define FinBit  0x80
#define MaskBit  0x80

static void websocket_close_response(int conn) {
  unsigned char close_packet[2] = {FinBit | OpCodeClose, 0x00};
  let xd = write(conn, close_packet, 2);
}
static int websocket_ping_response(int conn, uint64_t len, int mask, const unsigned char masking_key[4]) {
  unsigned char scratch[8] = {0};
  int i;
  // Immediately respond with a PONG containing the same data, but unmasked.
  scratch[0] = FinBit | OpCodePong;
  if (write(conn, scratch, 1) != 1) {
    console.log("write failed");
    return -1;
  }
  // Write the length
  if (len < 126) {
    scratch[0] = len & 0x7F;
    if (write(conn, scratch, 1) != 1) {
      console.log("write failed");
      return -1;
    }
  } else if (len < (1 << 16)) {
    scratch[0] = 126;
    if (write(conn, scratch, 1) != 1) {
      console.log("write failed");
      return -1;
    }
    scratch[0] = (len >> 8) & 0xFF;
    scratch[1] = len & 0xFF;
    if (write(conn, scratch, 2) != 2) {
      console.log("write failed");
      return -1;
    }
  } else {
    scratch[0] = 127;
    if (write(conn, scratch, 1) != 1) {
      console.log("write failed");
      return -1;
    }
    scratch[0] = (len >> 56) & 0xFF;
    scratch[1] = (len >> 48) & 0xFF;
    scratch[2] = (len >> 40) & 0xFF;
    scratch[3] = (len >> 32) & 0xFF;
    scratch[4] = (len >> 24) & 0xFF;
    scratch[5] = (len >> 16) & 0xFF;
    scratch[6] = (len >> 8) & 0xFF;
    scratch[7] = len & 0xFF;
    if (write(conn, scratch, 8) != 8) {
      console.log("write failed");
      return -1;
    }
  }
  // Write the unmasked data.
  for (i = 0; i < len; i++) {
    if (read(conn, scratch, 1) != 1) {
      console.log("write failed");
      return -1;
    }
    if (mask) scratch[0] ^= masking_key[i % 4];
    if (write(conn, scratch, 1) != 1) {
      console.log("write failed");
      return -1;
    }
  }
  return 0;
}
static char *recv_websocket_msg(int conn) {
  unsigned char scratch[8];

  int mask;
  unsigned char fin = 0;
  unsigned char opcode = 0;

  uint64_t len = 0ULL;
  unsigned char masking_key[4] = {0};

  uint64_t i;
  uint64_t current_offset = 0ULL;
  uint64_t next_offset;

  int first_frame = 1;
  size_t extra_len_bytes = 0;
  size_t bytes_read;
  char *data = calloc(2048, sizeof(char));

  loop {
    // Framing protocol for websockets (from the RFC)
    //  0                   1                   2                   3
    //  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    // +-+-+-+-+-------+-+-------------+-------------------------------+
    // |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
    // |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
    // |N|V|V|V|       |S|             |   (if payload len==126/127)   |
    // | |1|2|3|       |K|             |                               |
    // +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
    // |     Extended payload length continued, if payload len == 127  |
    // + - - - - - - - - - - - - - - - +-------------------------------+
    // |                               |Masking-key, if MASK set to 1  |
    // +-------------------------------+-------------------------------+
    // | Masking-key (continued)       |          Payload Data         |
    // +-------------------------------- - - - - - - - - - - - - - - - +
    // :                     Payload Data continued ...                :
    // + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
    // |                     Payload Data continued ...                |
    // +---------------------------------------------------------------+
    // Read the header all the way up to the beginning of Payload Data
    // First read the 2-byte required header
    if (read(conn, scratch, 2) != 2) {
      console.log("failed to read 2 bytes");
      return NULL;
    }

    fin = scratch[0] & FinBit;
    opcode = scratch[0] & 0x0F;
    mask = scratch[1] & MaskBit;

    // Length is either 7 bits, 16 bits, or 64 bits.
    len = scratch[1] & 0x7F;

    // Check for the signal values of len
    if (len == 126ULL) {
      extra_len_bytes = 2;
    } else if (len == 127ULL) {
      extra_len_bytes = 8;
    }
    // Read the extra length bytes, if required.
    if (extra_len_bytes > 0) {
      if (read(conn, scratch, extra_len_bytes) != extra_len_bytes) {
        console.log("failed to read %d bytes", extra_len_bytes);
        return NULL;
      }

      len = 0ULL;
      for (i = 0; i < extra_len_bytes; ++i) len = (len << 8) + scratch[i];
    }

    // Read the 4 byte mask, if required
    if (mask) {
      if (read(conn, masking_key, 4) != 4) {
        console.log("failed to read 4 bytes");
        return NULL;
      }
    } else {
      websocket_close_response(conn);
      return NULL;
    }
    // Do different things, depending on the received opcode.
    if (opcode == OpCodeClose) {
      websocket_close_response(conn);
      return NULL;
    } else if (opcode == OpCodePing) {
      if (websocket_ping_response(conn, len, mask, masking_key) != 0) {
        console.log("failed to send ping response");
        return NULL;
      }
    } else if (opcode == OpCodePong) {
      // Read the PONG. Ignore it.
      for (i = 0; i < len; i++) {
        if (read(conn, scratch, 1) != 1) {
          console.log("failed to read 1 byte");
          return NULL;
        }
      }
    } else if ((first_frame && opcode == OpCodeText) || (!first_frame && opcode == OpCodeContinue)) {
      // Read the frame data into memory
      if (read(conn, &(((char *) data)[current_offset]), (size_t) len) != len) {
        console.log("failed to read %d bytes", len);
        return NULL;
      }
      // Unmask the data, if required
      if (mask) {
        for (i = 0; i < len; i++) {
          ((unsigned char *) data)[current_offset + i] ^= masking_key[i % 4];
        }
      }
      // Increment our state
      current_offset = next_offset;
      first_frame = 0;
      if (fin) break;
    } else {
      console.log("Wrong stuff");
      return NULL;
    }
  }

  return data;
}

const struct datagrams_lib datagrams = {
        .read = recv_websocket_msg,
};
