#include "fft/event_require.h"
#include "psx/types.h"

/* Wait 16 frames, then stop the current thread. */
void require_thread_stop_after_16_frames(void) {
    battle_thread_wait_frames(0x10);
    require_thread_exit_current();
}
