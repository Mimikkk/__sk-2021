#pragma once
#include <server/responses/responses.h>

extern const struct sockets_lib {
    void (*close)(int);
    char *(*readline)(int fd);
    void (*send)(int fd, const char *);
} sockets;
