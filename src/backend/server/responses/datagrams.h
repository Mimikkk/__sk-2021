#pragma once
#include <stddef.h>

extern const struct datagrams_lib {
    char* (*read)(int);
} datagrams;
