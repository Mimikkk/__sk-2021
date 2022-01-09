#pragma once
#include <events/listeners/listeners.h>

extern const struct client_listener_t {
    Listener (*create)(void);
} client_listener;
