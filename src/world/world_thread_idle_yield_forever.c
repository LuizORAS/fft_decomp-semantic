#include "fft/world.h"

/* Idle thread body: yield forever. world_thread_start_idle_5_to_8 starts it;
 * world_thread_yield_forever is the same loop. */
void world_thread_idle_yield_forever(void) {
    for (;;) {
        world_thread_yield();
    }
}
