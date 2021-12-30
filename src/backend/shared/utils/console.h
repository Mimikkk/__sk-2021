#pragma once

extern const struct console_lib {
    void (*log)(const char *format, ...);
    void (*error)(const char *format, ...);
} console;
