#pragma once
#include <stddef.h>
#include <sys/epoll.h>
#include <stdbool.h>

typedef struct listener_t {
    bool should_exit;
    void (*on_input)(struct epoll_event);
    void (*on_output)(struct epoll_event);
    void (*on_error)(struct epoll_event);
    void (*on_hangup)(struct epoll_event);
} Listener;

extern const struct listeners_lib {
    Listener *(*get)(size_t);
    void (*set)(size_t, Listener);
    void (*remove)(size_t);
    void (*premature_exit)(size_t);
} listeners;
