#pragma once
#include <events/listeners/listeners.h>

extern const struct stdin_listener_t {
    Listener (*create)(void);
} stdin_listener;
