#include "fft/sound.h"
#include "fft/world.h"

void world_sound_queue_sfx_invalid(void) {
    g_sound_pending_sfx_id = MAIN_SFX_INVALID;
}
