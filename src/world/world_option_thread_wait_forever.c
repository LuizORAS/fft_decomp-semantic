#include "fft/thread.h"
#include "fft/world.h"

/* WORLD's copy of option_thread_wait_forever: yield every frame forever (the same loop as
 * world_thread_idle_yield_forever). */
void world_option_thread_wait_forever(void) {
    for (;;) {
        thread_yield();
    }
}
