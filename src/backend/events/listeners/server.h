#pragma once
#include <events/listeners/listeners.h>

extern const struct server_listener_t {
    Listener (*create)(void);
} server_listener;
