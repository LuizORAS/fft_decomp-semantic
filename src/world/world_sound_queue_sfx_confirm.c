#include "fft/world.h"

void world_sound_queue_sfx_confirm(void) {
    g_world_sound_pending_sfx_id = MAIN_SFX_CONFIRM;
}
