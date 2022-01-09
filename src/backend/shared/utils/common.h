#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define var __auto_type
#define let const var
#define SO_REUSEPORT 15
#define loop while (true)
#define noreturn _Noreturn void
static const int DefaultBufferSize = 2048;

extern char *str(const char *format, ...);
extern char *strip(const char *s);
extern bool starts_with(const char *prefix, const char *s);
