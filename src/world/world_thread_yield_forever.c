#include "fft/world.h"

/* Yield forever; the same loop as world_thread_idle_yield_forever. */
void world_thread_yield_forever(void) {
    for (;;) {
        world_thread_yield();
    }
}
