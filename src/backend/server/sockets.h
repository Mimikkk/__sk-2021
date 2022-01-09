#pragma once
#include <server/responses/responses.h>
#include <stddef.h>
#include <stdbool.h>

extern const struct sockets_lib {
    void (*close)(int);
    char *(*readline)(int);
    void (*send)(int, const char *);
    void (*read)(int, void *, int);
    bool (*try_read)(int, void *, int);
    bool (*try_send)(int, void *, int);
} sockets;
