#include <shared/utils/imports.h>
#include <shared/utils/error_handle.h>
#include "console.h"

static const int Success = 0;
static const int Failure = 1;

static void handle_exit(int exitcode) {
  if (exitcode != Success) console.error("Exiting due to error with code '%d'\n", exitcode);
  else console.info("Exiting successfully.\n", exitcode);
  exit(exitcode);
}
static bool handle_error(bool is_error, const char *message) {
  if (is_error) {
    console.error(message);
    handle_exit(Failure);
  }
  return true;
}

const struct quit_lib quit = {
        .on = handle_error,
        .now = handle_exit,
        .Success = Success,
        .Failure = Failure,
};