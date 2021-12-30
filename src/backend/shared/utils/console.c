#include <shared/utils/console.h>
#include <stdio.h>
#include <stdlib.h>
#include <shared/utils/common.h>
#include <stdarg.h>

static void printfln(const char *format, ...) {
  const char *fmt = str("%s\n", format);
  va_list arguments;
  va_start(arguments, format);
  vfprintf(stdout, fmt, arguments);
  va_end(arguments);
  free((void *) fmt);
}
static void iprintfln(const char *format, ...) {
  const char *fmt = str("Info: %s\n", format);
  va_list arguments;
  va_start(arguments, format);
  vfprintf(stdout, fmt, arguments);
  va_end(arguments);
  free((void *) fmt);
}
static void eprintfln(const char *format, ...) {
  const char *fmt = str("Error: %s\n", format);
  va_list arguments;
  va_start(arguments, format);
  vfprintf(stderr, fmt, arguments);
  va_end(arguments);
  free((void *) fmt);
}

const struct console_lib console = {
        .log = printfln,
        .error = eprintfln,
        .info = iprintfln,
};