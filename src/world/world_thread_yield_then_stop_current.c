#include "fft/thread.h"
#include "fft/world.h"

/* Give other event work one turn before terminating this thread. */
void world_thread_yield_then_stop_current(void) {
    thread_yield();
    thread_exit_current();
}
