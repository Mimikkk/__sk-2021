#pragma once
#include <stdbool.h>

#define var __auto_type
#define let const var
#define SO_REUSEPORT 15
#define loop while (true)
#define noreturn _Noreturn void

extern char *str(const char *format, ...);
extern char *strip(const char *str);
