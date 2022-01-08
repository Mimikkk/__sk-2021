#pragma once
#include <stddef.h>

extern const struct datagrams_lib {
    char *(*try_read)(int);
    void (*write)(int, const char *);
} datagrams;
