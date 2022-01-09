#pragma once

typedef char *response_t;

extern const struct responses_lib {
    response_t (*create)(const char *);
    void (*add_header)(const char *, response_t *);
    void (*send)(response_t, int);
    response_t (*handshake)(const char *key);
} responses;
