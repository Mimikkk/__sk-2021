#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct thread_t {
    uint64_t system_id;
    const uint64_t id;
} Thread;

extern const struct thread_lib {
    Thread (*create)(void *);
    bool (*is_alive)(const Thread);
    void (*join)(const Thread);
    void (*kill)(const Thread);
} threads;
