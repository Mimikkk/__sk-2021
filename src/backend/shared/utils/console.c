#include <shared/utils/console.h>
#include <stdio.h>
#include <stdlib.h>
#include <shared/utils/common.h>
#include <stdarg.h>
#include <shared/utils/colors.h>

static void printfln(const char *format, ...) {
  const char *fmt = str("%s%s%s.%s\n",
                        colors.Reset, format, colors.Yellow, colors.Reset);
  va_list arguments;
  va_start(arguments, format);
  vfprintf(stdout, fmt, arguments);
  va_end(arguments);
  free((void *) fmt);
}
static void iprintfln(const char *format, ...) {
  const char *fmt = str("%sInfo%s:%s %s%s.%s\n",
                        colors.Gray, colors.Yellow, colors.Gray, format, colors.Yellow, colors.Reset);
  va_list arguments;
  va_start(arguments, format);
  vfprintf(stdout, fmt, arguments);
  va_end(arguments);
  free((void *) fmt);
}
static void eprintfln(const char *format, ...) {
  const char *fmt = str("%sError%s:%s %s%s.%s\n",
                        colors.Red, colors.Yellow, colors.Reset, format, colors.Yellow, colors.Reset);
  va_list arguments;
  va_start(arguments, format);
  vfprintf(stderr, fmt, arguments);
  va_end(arguments);
  free((void *) fmt);
}
static void evprintfln(const char *format, ...) {
  const char *fmt = str("%sEvent%s:%s %s%s.%s\n",
                        colors.Green, colors.Yellow, colors.Reset, format, colors.Yellow, colors.Reset);
  va_list arguments;
  va_start(arguments, format);
  vfprintf(stderr, fmt, arguments);
  va_end(arguments);
  free((void *) fmt);
}
static void stprintfln(const char *format, ...) {
  const char *fmt = str("%sStatistics%s:%s %s%s.%s\n",
                        colors.Magenta, colors.Yellow, colors.Reset, format, colors.Yellow, colors.Reset);
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
        .event = evprintfln,
        .stat = stprintfln,
};