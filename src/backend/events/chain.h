#pragma once

extern const struct chain_lib {
    void (*start)(void);
    void (*stop)(void);
    int *fd;
} chains;
