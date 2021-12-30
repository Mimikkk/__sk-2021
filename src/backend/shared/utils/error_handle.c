#include <shared/utils/imports.h>
#include <shared/utils/error_handle.h>

static void handle_exit(int exitcode) {
  if (exitcode > 0) printf("Exiting due to error with code '%d'\n", exitcode);
  exit(exitcode);
}
static bool handle_error(bool is_error, const char *message) {
  if (is_error) {
    fprintf(stderr, "Error: %s\n", message);
    handle_exit(1);
  }
  return true;
}

const struct quit_lib quit = {
        .on = handle_error,
        .graceful = handle_exit,
        .Success = 0,
        .Failure = 1,
};