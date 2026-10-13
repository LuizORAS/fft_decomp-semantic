#include "fft/battle_sound.h"

void sound_queue_sfx_invalid(void) {
    g_sound_pending_sfx_id = MAIN_SFX_INVALID;
}
