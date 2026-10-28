#include "fft/main.h"
#include "fft/sound.h"

/* Queue the Confirm sound (MAIN_SFX_CONFIRM) for this frame, as sound_queue_sfx_cancel
 * does. */
void sound_queue_sfx_confirm(void) {
    g_sound_pending_sfx_id = MAIN_SFX_CONFIRM;
}
