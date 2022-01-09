#pragma once
#include <stddef.h>

extern const struct datagrams_lib {
    char *(*try_read)(int);
    char *(*read)(int);
    void (*write)(int, const char *);
} datagrams;
