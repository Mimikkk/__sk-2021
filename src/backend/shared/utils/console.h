#pragma once

extern const struct console_lib {
    void (*log)(const char *format, ...);
    void (*info)(const char *format, ...);
    void (*event)(const char *format, ...);
    void (*error)(const char *format, ...);
} console;
