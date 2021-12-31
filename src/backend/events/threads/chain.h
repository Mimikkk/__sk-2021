#pragma once

extern const struct chain_lib {
    void (*start)(void);
    int *fd;
} chains;
