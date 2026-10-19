#include "fft/thread.h"
#include "psx/types.h"

/* Yield frames times: one scheduler pass, so one frame, each. */
void thread_wait_frames(s32 frames) {
    s32 i;

    for (i = 0; i < frames; i++) {
        thread_yield();
    }
}
