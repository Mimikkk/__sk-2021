#include <statistics/statistics.h>
#include <shared/utils/console.h>
#include <shared/utils/common.h>
#include <unistd.h>
#include <shared/utils/colors.h>
#include <events/listeners/listeners.h>
#include <stdlib.h>

enum { Interval = 25 };

static uint64_t total_connections = 0;
static uint64_t current_connections = 0;
static uint64_t total_hangup_events = 0;
static uint64_t total_write_events = 0;
static uint64_t total_read_events = 0;
static uint64_t total_error_events = 0;
static uint64_t total_events = 0;

static void display_server_statistics(void) {
  let names = listeners.names_joined();

  console.stat("┼── Start");
  console.stat("Connections: %s%d%s/%s%d%s",
               colors.Green, current_connections, colors.Reset,
               colors.Yellow, total_connections, colors.Reset);
  console.stat("Connected users: %s", names ? names : "none");
  console.stat("Total events %d [%swrite %s%d%s|%sread %s%d%s|%serror %s%d%s|%shangup %s%d%s]",
               total_events,
               colors.Gray, colors.Blue, total_write_events, colors.Reset,
               colors.Gray, colors.Green, total_read_events, colors.Reset,
               colors.Gray, colors.Red, total_error_events, colors.Reset,
               colors.Gray, colors.Yellow, total_hangup_events, colors.Reset);
  console.stat("┼── End");
}

static noreturn start(void) {
  console.stat("Starting server statistics");

  loop {
    display_server_statistics();
    sleep(Interval);
  }
}

const struct statistics_lib statistics = {
        .start = start,
        .total_connections = &total_connections,
        .current_connections = &current_connections,

        .total_events = &total_events,
        .total_hangup_events = &total_hangup_events,
        .total_write_events = &total_write_events,
        .total_read_events = &total_read_events,
        .total_error_events = &total_error_events,
};
