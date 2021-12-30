#include <shared/utils/common.h>
#include <shared/utils/imports.h>

const char *str(const char *fmt, ...) {
  va_list arguments;
  va_start(arguments, fmt);
  size_t size = snprintf(NULL, 0, fmt, arguments);
  char *buffer = (char *) malloc(size + 1);
  vsprintf(buffer, fmt, arguments);
  va_end(arguments);
  return buffer;
}
