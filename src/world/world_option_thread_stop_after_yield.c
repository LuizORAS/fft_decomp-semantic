#include "fft/thread.h"
#include "fft/world.h"

/* WORLD's copy of option_thread_stop_after_yield: yield once, then exit the current thread. Nothing
 * references it (QUIRKS.md). */
void world_option_thread_stop_after_yield(void) {
    thread_yield();
    thread_exit_current();
}
