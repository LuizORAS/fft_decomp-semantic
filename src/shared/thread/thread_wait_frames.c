#include "fft/thread.h"
#include "psx/types.h"

void thread_wait_frames(s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        thread_yield();
    }
}
