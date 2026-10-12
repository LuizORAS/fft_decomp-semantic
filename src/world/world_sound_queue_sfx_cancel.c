#include "fft/world.h"

void world_sound_queue_sfx_cancel(void) {
    g_world_sound_pending_sfx_id = MAIN_SFX_CANCEL;
}
