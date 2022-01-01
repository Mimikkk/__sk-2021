#pragma once
#include <stdbool.h>

extern const struct events_lib {
    void (*add)(int, uint32_t);
    void (*remove)(int);

    char *(*info)(struct epoll_event);
    void (*await)(void);
    void (*handle)(void);
} events;
