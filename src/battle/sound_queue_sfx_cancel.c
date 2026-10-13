#include "fft/battle_sound.h"
#include "fft/sound.h"

void sound_queue_sfx_cancel(void) {
    g_sound_pending_sfx_id = MAIN_SFX_CANCEL;
}
