#include "fft/world.h"
#include "psx/types.h"

/* Yield frames times: one scheduler pass, so one frame, each. */
void world_thread_wait_frames(s32 frames) {
    s32 elapsed = 0;

    while (elapsed < frames) {
        world_thread_yield();
        elapsed++;
    }
}
