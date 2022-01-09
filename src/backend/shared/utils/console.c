#include <shared/utils/console.h>
#include <stdio.h>
#include <stdlib.h>
#include <shared/utils/common.h>
#include <stdarg.h>
#include <shared/utils/colors.h>

#define printfln(fmt) \
  va_list arguments; \
  va_start(arguments, format); \
  vfprintf(stdout, fmt, arguments); \
  va_end(arguments); \
  free((void *) fmt) \

static void lprintfln(const char *format, ...) {
  printfln(str("%s%s%s.%s\n",
               colors.Reset, format, colors.Yellow, colors.Reset));
}
static void iprintfln(const char *format, ...) {
  printfln(str("%sInfo%s:%s %s%s.%s\n",
               colors.Gray, colors.Yellow, colors.Gray, format, colors.Yellow, colors.Reset));
}
static void eprintfln(const char *format, ...) {
  printfln(str("%sError%s:%s %s%s.%s\n",
               colors.Red, colors.Yellow, colors.Reset, format, colors.Yellow, colors.Reset));
}
static void evprintfln(const char *format, ...) {
  printfln(str("%sEvent%s:%s %s%s.%s\n",
               colors.Green, colors.Yellow, colors.Reset, format, colors.Yellow, colors.Reset));
}
static void stprintfln(const char *format, ...) {
  printfln(str("%sStatistics%s:%s %s%s.%s\n",
               colors.Magenta, colors.Yellow, colors.Reset, format, colors.Yellow, colors.Reset));
}

const struct console_lib console = {
        .log = lprintfln,
        .error = eprintfln,
        .info = iprintfln,
        .event = evprintfln,
        .stat = stprintfln,
};