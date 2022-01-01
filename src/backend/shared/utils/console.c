#include <shared/utils/console.h>
#include <stdio.h>
#include <stdlib.h>
#include <shared/utils/common.h>
#include <stdarg.h>

static const char *const Reset = "\e[0m";
static const char *const Gray = "\e[38;5;8m";
static const char *const Silver = "\e[38;5;7m";
static const char *const Cyan = "\e[38;5;6m";
static const char *const Magenta = "\e[38;5;5m";
static const char *const Blue = "\e[38;5;4m";
static const char *const Yellow = "\e[38;5;3m";
static const char *const Green = "\e[38;5;2m";
static const char *const Red = "\e[38;5;1m";

static void printfln(const char *format, ...) {
  const char *fmt = str("%s%s%s.%s\n", Blue, format, Yellow, Reset);
  va_list arguments;
  va_start(arguments, format);
  vfprintf(stdout, fmt, arguments);
  va_end(arguments);
  free((void *) fmt);
}
static void iprintfln(const char *format, ...) {
  const char *fmt = str("%sInfo%s:%s %s%s.%s\n", Gray, Yellow, Gray, format, Yellow, Reset);
  va_list arguments;
  va_start(arguments, format);
  vfprintf(stdout, fmt, arguments);
  va_end(arguments);
  free((void *) fmt);
}
static void eprintfln(const char *format, ...) {
  const char *fmt = str("%sError%s:%s %s%s.%s\n", Red, Yellow, Reset, format, Yellow, Reset);
  va_list arguments;
  va_start(arguments, format);
  vfprintf(stderr, fmt, arguments);
  va_end(arguments);
  free((void *) fmt);
}
static void evprintfln(const char *format, ...) {
  const char *fmt = str("%sEvent%s:%s %s%s.%s\n", Green, Yellow, Reset, format, Yellow, Reset);
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
        .event = evprintfln
};