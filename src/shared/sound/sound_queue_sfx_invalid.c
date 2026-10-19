#include "fft/main.h"
#include "fft/sound.h"

/* Queue the Invalid sound (MAIN_SFX_INVALID) for this frame, as sound_queue_sfx_cancel
 * does. */
void sound_queue_sfx_invalid(void) {
    g_sound_pending_sfx_id = MAIN_SFX_INVALID;
}
