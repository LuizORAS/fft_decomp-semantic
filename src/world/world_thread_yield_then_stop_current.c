#include "fft/world.h"

/* Yield once, then exit the current thread. */
void world_thread_yield_then_stop_current(void) {
    world_thread_yield();
    world_thread_exit_current();
}
