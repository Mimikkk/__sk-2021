#pragma once

extern const struct sockets_lib {
    char *(*readline)(int fd);
} sockets;
