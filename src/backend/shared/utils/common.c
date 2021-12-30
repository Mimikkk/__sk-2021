#include <shared/utils/common.h>
#include <shared/utils/imports.h>

const char *str(const char *format, ...) {
  va_list arguments, size_check;
  va_start(arguments, format);
  va_copy(size_check, arguments);
  char *buffer = (char *) malloc(vsnprintf(NULL, 0, format, size_check));
  vsprintf(buffer, format, arguments);
  va_end(arguments);
  return buffer;
}
