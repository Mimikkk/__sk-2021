#pragma once

extern const struct server_lib {
    void (*open)(void);
    void (*close)(void);
    const char *(*info)(void);
    int (*accept)(void);

    const int *socket;
    const struct sockaddr_in *address;
    const char **raw_address;
    int *raw_port;
    const int max_connections;
} server;
