#pragma once
#include <stdbool.h>

extern const struct quit_lib {
    bool (*on)(bool, const char *);
    void (*now)(int);
    const int Success;
    const int Failure;
} quit;
