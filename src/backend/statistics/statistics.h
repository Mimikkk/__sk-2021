#pragma once

#include <stdint-gcc.h>
extern const struct statistics_lib {
    void (*start)(void);

    uint64_t *total_connections;
    uint64_t *current_connections;
    uint64_t *total_hangup_events;
    uint64_t *total_write_events;
    uint64_t *total_read_events;
    uint64_t *total_error_events;
    uint64_t *total_events;
} statistics;
