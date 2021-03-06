#pragma once
#include <stddef.h>
#include <sys/epoll.h>
#include <stdbool.h>
#include <server/responses/responses.h>

typedef struct listener_info_t {
    response_t response;
    const char *name;
    bool should_exit;
    int fd;
} ListenerInfo;

typedef struct listener_t {
    void (*on_read)(struct epoll_event);
    void (*on_write)(struct epoll_event);
    void (*on_error)(struct epoll_event);
    void (*on_hangup)(struct epoll_event);
    struct listener_info_t info;
} Listener;

extern const struct listeners_lib {
    Listener *(*get)(int);
    Listener *(*get_by_name)(const char *);

    void (*set)(int, Listener);
    Listener (*empty)(void);
    void (*clear)(int);

    void (*premature_exit)(int);
    bool (*contains_name)(const char *);
    char *(*names_joined)(void);
} listeners;
