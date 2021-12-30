#pragma once

extern const struct server_lib {
    void (*open)(void);
    void (*close)(void);
    const char *(*info)(void);

    int *socket;
    struct sockaddr_in *address;
    int port;
    int max_connections;
} server;
