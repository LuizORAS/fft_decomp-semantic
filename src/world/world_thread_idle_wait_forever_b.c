#include "fft/world.h"
#include "psx/types.h"

/* Same as world_thread_idle_wait_forever, at its own address. */
void world_thread_idle_wait_forever_b(void) {
    for (;;) {
        world_thread_wait_frames(1);
    }
}
