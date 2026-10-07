#include "fft/world.h"
#include "psx/types.h"

/* Idle thread body: wait one frame, forever; world_thread_idle_wait_forever_b is the
 * same loop. */
void world_thread_idle_wait_forever(void) {
    for (;;) {
        world_thread_wait_frames(1);
    }
}
