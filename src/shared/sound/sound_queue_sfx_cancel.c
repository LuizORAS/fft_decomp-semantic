#include "fft/main.h"
#include "fft/sound.h"

/* Queue the Cancel sound (MAIN_SFX_CANCEL) for this frame in g_sound_pending_sfx_id:
 * battle_script_process_pending_requests plays it after the threads have run, world_sound_dispatch_effect
 * in WORLD's copy. */
void sound_queue_sfx_cancel(void) {
    g_sound_pending_sfx_id = MAIN_SFX_CANCEL;
}
