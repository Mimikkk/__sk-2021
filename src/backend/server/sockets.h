#pragma once
#include <server/responses/responses.h>
#include <stddef.h>

extern const struct sockets_lib {
    void (*close)(int);
    char *(*readline)(int);
    void (*send)(int, const char *);
    void (*read)(int, void *, size_t);
} sockets;
