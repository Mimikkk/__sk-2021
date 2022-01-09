#pragma once

extern const struct encryption_lib {
    char *(*encode_base64)(const char *);
    char *(*decode_base64)(const char *);
    char *(*encode_sha1)(const char *);
    char *(*websocket)(const char *);
} encryption;
