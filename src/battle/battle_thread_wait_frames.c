#include "fft/battle.h"
#include "psx/types.h"

/* Yield frames times: one scheduler pass, so one frame, each. */
void battle_thread_wait_frames(s32 frames) {
    s32 i;

    for (i = 0; i < frames; i++) {
        battle_thread_yield();
    }
}
