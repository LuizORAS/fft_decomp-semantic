#include "fft/world.h"
#include "psx/types.h"

void world_sound_set_effect_to_confirm(void) {
    g_world_sound_pending_sfx_id = MAIN_SFX_CONFIRM;
}
