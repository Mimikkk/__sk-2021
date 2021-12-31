#pragma once
#include <stdbool.h>

extern const struct events_lib {
    void (*add)(int, uint32_t);
    void (*remove)(int);
    void (*await)(void);
    void (*handle)(struct epoll_event);
    char *(*info)(struct epoll_event);

    size_t *awaited_count;
    struct epoll_event *awaited;
} events;
