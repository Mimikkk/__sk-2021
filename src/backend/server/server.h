#pragma once

extern const struct server_lib {
    void (*open)(void);
    void (*close)(void);
    const char *(*info)(void);

    const int *socket;
    const struct sockaddr_in *address;
    const int port;
    const int max_connections;
} server;
