#include "fft/world.h"

/* Give other event work one turn before terminating this thread. */
void world_thread_yield_then_stop_current(void) {
    world_thread_yield();
    world_thread_exit_current();
}
