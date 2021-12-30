#include <shared/utils/console.h>
#include <stdio.h>
#include <stdlib.h>
#include <shared/utils/common.h>

static void printfln(const char *format, ...) {
  const char *s = str(format);
  fprintf(stdout, "%s\n", s);
  free((void *) s);
}
static void eprintfln(const char *format, ...) {
  const char *s = str(format);
  fprintf(stderr, "%s\n", s);
  free((void *) s);
}

const struct console_lib console = {
        .log = printfln,
        .error = eprintfln,
};