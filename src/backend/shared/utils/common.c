#include <shared/utils/common.h>
#include <shared/imports.h>
#include <ctype.h>

char *strip(const char *s) {
  size_t size = strlen(s);
  var copy = str(s);
  if (!size) return copy;

  char *end = copy + size - 1;
  while (end >= copy && isspace(*end)) end--;

  *(end + 1) = '\0';
  while (*copy && isspace(*copy)) copy++;

  let result = str(copy);
  return result;
}

char *str(const char *format, ...) {
  va_list arguments, size_check;
  va_start(arguments, format);
  va_copy(size_check, arguments);
  size_t size = vsnprintf(NULL, 0, format, size_check);
  char *buffer = (char *) malloc(size + 1);
  buffer[0] = '\0';
  vsprintf(buffer, format, arguments);
  buffer[size] = '\0';
  va_end(arguments);
  return buffer;
}

bool starts_with(const char *prefix, const char *s) {
  size_t str_length = strlen(s);
  size_t prefix_length = strlen(prefix);
  return str_length < prefix_length ? false : memcmp(prefix, s, prefix_length) == 0;
}